#include <Tasks/stdio_compiler_task.h>
#include <sys/resource.h>
#include <cstring>
#include <cerrno>

bool stdio_compiler_task::check_permissions()
{
    if (compile_command.empty() || source_file_name.empty() || output_file_name.empty())
    {
        return false;
    }
    if (submission_id.empty())
    {
        return false;
    }
    if (time_limit <= 0 || exec_size_limit <= 0)
    {
        return false;
    }
    return true;
}

result_enum stdio_compiler_task::execute(pthread_t thread_id, int user_id)
{
    (void)thread_id;

    if (!check_permissions())
    {
      LOG_ERROR_USER(user_id, "Permission check failed");
      return result_enum::FAIL;
    }
    if (user_id <= 0)
    {
      LOG_ERROR_USER(user_id, "Invalid user ID");
      return result_enum::FAIL;
    }

    const std::string sandbox_path = architecture_utilities::get_sandbox_path();
    if (sandbox_path.empty())
    {
        LOG_ERROR_USER(user_id, "Sandbox path is not set in environment variables");
        return result_enum::FAIL;
    }

    const std::string run_username = architecture_utilities::get_weak_user(user_id);
    const std::string run_dir = architecture_utilities::get_run_dir(user_id);

    const std::string source_host_path = architecture_utilities::get_submission_source_path(submission_id);  
    const std::string output_host_path = architecture_utilities::get_submission_exec_path(submission_id);  
    const std::string source_run_path = run_dir + "/" + source_file_name;
    const std::string output_run_path = run_dir + "/" + output_file_name;

    struct passwd pw_struct;
    struct passwd *pw;
    char pw_buf[8192];
    int pw_res = getpwnam_r(run_username.c_str(), &pw_struct, pw_buf, sizeof(pw_buf), &pw);
    if (pw_res != 0 || pw == nullptr)
    {
      LOG_ERROR_USER(user_id, "Failed to get user information for sandbox user");
      return result_enum::FAIL;
    }

    if (!general_utilities::copy_file(source_host_path, source_run_path, 0644))
    {
      LOG_ERROR_USER(user_id, "Couldn't copy source file to run directory");
      return result_enum::FAIL;
    }

    unlink(output_run_path.c_str());

    const auto started_at = std::chrono::steady_clock::now();
    pid_t pid = fork();
    if (pid < 0)
    {
        LOG_ERROR_USER(user_id, "Couldn't fork");
        return result_enum::FAIL;
    }

    if (pid == 0)
    {
        setpgid(0, 0);

        //daca vrem chroot trebe sa includem niste librarii in plus aduse aici, eventual mutam chroot in wrapperu de la comanda, dar again nu e necesar ca runneru oricum e jailed. adica e problema de user experience
        // g++: fatal error: cannot execute 'cc1plus': posix_spawnp: No such file or directory
        // if (!architecture_utilities::change_root_to_sandbox())
        // {
        //   print_error(thread_id, user_id, "Failed to change root to sandbox");
        //   _exit(127);
        // }

        if (!architecture_utilities::change_dir_to_user(user_id))
        {
            LOG_ERROR_USER(user_id, "Failed to change directory to user's run directory");
            _exit(127);
        }


        int null_fd = open("/dev/null", O_RDONLY);
        if (null_fd >= 0)
        {
            dup2(null_fd, STDIN_FILENO);
            close(null_fd);
        }

        std::string source_in_jail = source_file_name;
        std::string output_in_jail = output_file_name;

        char *const argv[] = {
            const_cast<char *>(compile_command.c_str()),
            const_cast<char *>("-std=c++17"),
            const_cast<char *>("-O2"),
            const_cast<char *>("-o"),
            const_cast<char *>(output_in_jail.c_str()),
            const_cast<char *>(source_in_jail.c_str()),
            nullptr};

        execv(compile_command.c_str(), argv);
        _exit(127);
    }

    setpgid(pid, pid);

    int status = 0;
    bool timed_out = false;

    // get rusage for this specific child using wait4
    long long compile_time_ms = -1;
    struct rusage ru;
    while (true)
    {
        pid_t waited = wait4(pid, &status, WNOHANG, &ru);
        if (waited == pid)
        {
            long long us = (long long)ru.ru_utime.tv_sec * 1000000LL + ru.ru_utime.tv_usec
                + (long long)ru.ru_stime.tv_sec * 1000000LL + ru.ru_stime.tv_usec;
            compile_time_ms = us / 1000;
            break;
        }
        else if (waited == 0)
        {
            const auto now = std::chrono::steady_clock::now();
            const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - started_at).count();
            if (elapsed_ms > (long long)time_limit)
            {
                timed_out = true;
                killpg(pid, SIGKILL);
                // reap and collect rusage for the killed child
                pid_t w = wait4(pid, &status, 0, &ru);
                if (w == pid) {
                    long long us = (long long)ru.ru_utime.tv_sec * 1000000LL + ru.ru_utime.tv_usec
                        + (long long)ru.ru_stime.tv_sec * 1000000LL + ru.ru_stime.tv_usec;
                    compile_time_ms = us / 1000;
                } else {
                    compile_time_ms = elapsed_ms;
                }
                break;
            }
            usleep(1000);
            continue;
        }
        else
        {
            if (errno == EINTR) continue;
            // error while waiting; try to reap and collect rusage
            if (wait4(pid, &status, 0, &ru) == pid) {
                long long us = (long long)ru.ru_utime.tv_sec * 1000000LL + ru.ru_utime.tv_usec
                    + (long long)ru.ru_stime.tv_sec * 1000000LL + ru.ru_stime.tv_usec;
                compile_time_ms = us / 1000;
            }
            LOG_ERROR_USER(user_id, "Error while waiting for compiler process");
            return result_enum::FAIL;
        }
    }

    if (timed_out)
    {
        return result_enum::TLE;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        return result_enum::CPE;
    }

    struct stat st;
    if (stat(output_run_path.c_str(), &st) != 0)
    {
        return result_enum::CPE;
    }
    if ((long)st.st_size > exec_size_limit)
    {
        unlink(output_run_path.c_str());
        return result_enum::CPE;
    }

    if (rename(output_run_path.c_str(), output_host_path.c_str()) != 0)
    {
        if (!general_utilities::copy_file(output_run_path, output_host_path, 0755))
        {
            return result_enum::FAIL;
        }
        unlink(output_run_path.c_str());
    }

    chmod(output_host_path.c_str(), 0755);
    LOG_INFO_USER(user_id, "Compilation completed for submission " + submission_id + " with result " + general_utilities::enum_to_string(result_enum::OK) + " in " + std::to_string(compile_time_ms) + "ms, time_limit: " + std::to_string((long long)time_limit) + "ms");
    return result_enum::OK;
}

