#include <Tasks/stdio_compiler_task.h>

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

result_enum stdio_compiler_task::execute(int thread_id, int user_id)
{
    (void)thread_id;

    if (!check_permissions())
    {
        return result_enum::FAIL;
    }
    if (user_id <= 0)
    {
        return result_enum::FAIL;
    }

    const char *sandbox_path = getenv("SANDBOX_PATH");
    if (sandbox_path == nullptr || sandbox_path[0] == '\0')
    {
        return result_enum::FAIL;
    }

    const std::string run_username = "amarat" + std::to_string(user_id);
    const std::string run_dir = submission_info_utilities::get_run_dir(run_username);

    const std::string source_host_path = submission_info_utilities::get_submission_source_path(submission_id);  
    const std::string output_host_path = submission_info_utilities::get_submission_exec_path(submission_id);  
    const std::string source_run_path = run_dir + "/" + "source";
    const std::string output_run_path = run_dir + "/" + "executable";

    struct passwd *pw = getpwnam(run_username.c_str());
    if (pw == nullptr)
    {
        return result_enum::FAIL;
    }

    if (!utilities::copy_file(source_host_path, source_run_path, 0644))
    {
        return result_enum::FAIL;
    }

    unlink(output_run_path.c_str());

    const auto started_at = std::chrono::steady_clock::now();
    pid_t pid = fork();
    if (pid < 0)
    {
        return result_enum::FAIL;
    }

    if (pid == 0)
    {
        setpgid(0, 0);

        if (chdir(run_dir.c_str()) != 0)
        {
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

    while (true)
    {
        pid_t waited = waitpid(pid, &status, WNOHANG);
        if (waited == pid)
        {
            break;
        }
        if (waited < 0)
        {
            killpg(pid, SIGKILL);
            waitpid(pid, &status, 0);
            return result_enum::FAIL;
        }

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - started_at).count();
        if (elapsed_ms > (long long)time_limit)
        {
            timed_out = true;
            killpg(pid, SIGKILL);
            waitpid(pid, &status, 0);
            break;
        }

        usleep(1000);
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
        if (!utilities::copy_file(output_run_path, output_host_path, 0755))
        {
            return result_enum::FAIL;
        }
        unlink(output_run_path.c_str());
    }

    chmod(output_host_path.c_str(), 0755);
    return result_enum::OK;
}
