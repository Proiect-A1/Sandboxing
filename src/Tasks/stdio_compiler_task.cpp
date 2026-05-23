#include <Tasks/stdio_compiler_task.h>
#include <sys/resource.h>
#include <cstring>
#include <cerrno>
#include <cmath>

bool stdio_compiler_task::check_permissions(int user_id)
{
    (void)user_id;
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

    if (!check_permissions(user_id))
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
    const std::string run_dir = architecture_utilities::get_run_dir_absolute_path(user_id);


    const std::string source_run_path = run_dir + "/" + source_file_name;
    const std::string output_run_path = run_dir + "/" + output_file_name;

struct passwd pw_struct;
  struct passwd *pwp;
  char pw_buf[8192];
  int pw_res = getpwnam_r(run_username.c_str(), &pw_struct, pw_buf, sizeof(pw_buf), &pwp);
  if (pw_res != 0 || pwp == nullptr)
  {
    LOG_ERROR_USER(user_id, "Failed to get user information for sandbox user");
    return result_enum::FAIL;
  }
  struct passwd pw = pw_struct;

  LOG_DEBUG_USER(user_id, "Preparing to fork");
    
  pthread_mutex_lock(&Logger::mtx);
    pid_t pid = fork();
    if (pid < 0)
    {
        pthread_mutex_unlock(&Logger::mtx);
        LOG_ERROR_USER(user_id, "Couldn't fork");
        return result_enum::FAIL;
    }

    if (pid == 0)
    {
      for (int i = 4; i <= 1024; i++) close(i);
        pthread_mutex_unlock(&Logger::mtx);
        setpgid(0, 0);

        //daca vrem chroot trebe sa includem niste librarii in plus aduse aici, eventual mutam chroot in wrapperu de la comanda, dar again nu e necesar ca runneru oricum e jailed. adica e problema de user experience
        // g++: fatal error: cannot execute 'cc1plus': posix_spawnp: No such file or directory
       
       //ROBERT ITI DAI FORMAT SINGUR LA COD
        if (initgroups(run_username.c_str(), pw.pw_gid) != 0)
        {
        LOG_ERROR_USER(user_id, "Failed to initialize group access inside sandbox");
        _exit(127);
        }
    
    // if (!(architecture_utilities::change_root_to_sandbox()))
    // {
      // LOG_ERROR_USER(user_id, "Failed to change root to sandbox");
    //   _exit(127);
    // }
    
    // std::string inner_run_dir = architecture_utilities::get_run_dir_relative_to_sandbox_path(user_id);
    std::string inner_run_dir = architecture_utilities::get_run_dir_absolute_path(user_id);
    if (chdir(inner_run_dir.c_str()) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to change directory to run directory inside sandbox");
      _exit(127);
    }

    // LOG_DEBUG_USER(user_id, "Changed directory to run directory inside sandbox: " + inner_run_dir + " : " +general_utilities::syscall_to_string("ls"));
    
    // if (setgid(pw.pw_gid) != 0)
    // {
      // LOG_ERROR_USER(user_id, "Failed to set group ID inside sandbox");
    //   _exit(127);
    // }
    
    // if (setuid(pw.pw_uid) != 0)
    // {
      // LOG_ERROR_USER(user_id, "Failed to set user ID inside sandbox");
    //   _exit(127);
    // }


        int null_fd = open("/dev/null", O_RDONLY);
        if (null_fd >= 0)
        {
            dup2(null_fd, STDIN_FILENO);
            close(null_fd);
        }

        setenv("TMPDIR", ".", 1);

        std::string source_in_jail = source_file_name;
        std::string output_in_jail = output_file_name;

        char **argv = new char *[arguments.size() + 1];
        for (size_t i = 0; i < arguments.size(); ++i)
        {
            argv[i] = const_cast<char *>(arguments[i].c_str());
        }
        argv[arguments.size()] = nullptr;

        long sec = (long)std::ceil(time_limit / 1000.0f);
        if (sec <= 0) sec = 1;
        struct rlimit cpu_rl;
        cpu_rl.rlim_cur = (rlim_t)sec;
        cpu_rl.rlim_max = (rlim_t)sec;
        if (setrlimit(RLIMIT_CPU, &cpu_rl) != 0)
        {
            LOG_ERROR_USER(user_id, "Failed to set CPU time limit");
            _exit(127);
        }
        LOG_DEBUG_USER(user_id, "Compiler has reached exec");
        execv(compile_command.c_str(), argv);
        execv(compile_command.c_str(), argv);
        execv(compile_command.c_str(), argv);
        execv(compile_command.c_str(), argv);
        execv(compile_command.c_str(), argv);
        LOG_ERROR_USER(user_id, "Failed to execute compile command");
        _exit(127);
    }
    pthread_mutex_unlock(&Logger::mtx);

    setpgid(pid, pid);

    int status = 0;

    long long compile_time_ms = -1;
    struct rusage ru;
    while (true)
    {
        pid_t waited = wait4(pid, &status, WNOHANG, &ru);
        if (waited == pid)
        {
            long long us = (long long)ru.ru_utime.tv_sec * 1000000LL + ru.ru_utime.tv_usec;
            compile_time_ms = us / 1000;
            break;
        }
        else if (waited == 0)
        {
            usleep(1000);
            continue;
        }
        else
        {
            if (errno == EINTR) continue;
            if (wait4(pid, &status, 0, &ru) == pid) {
                long long us = (long long)ru.ru_utime.tv_sec * 1000000LL + ru.ru_utime.tv_usec;
                compile_time_ms = us / 1000;
            }
            LOG_ERROR_USER(user_id, "Error while waiting for compiler process");
            return result_enum::FAIL;
        }
    }

    if (WIFSIGNALED(status))
    {
        LOG_ERROR_USER(user_id, "Compiler process was killed by signal: " + std::to_string(WTERMSIG(status)));
        return result_enum::CPE;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
        LOG_DEBUG_USER(user_id, "Compiler process exited with non-zero status: " + std::to_string(WEXITSTATUS(status)));
        if (WEXITSTATUS(status) == 127){
          return result_enum::FAIL;
        }
        return result_enum::CPE;
    }

    struct stat st;
    if (stat(output_run_path.c_str(), &st) != 0)
    {
        LOG_DEBUG_USER(user_id, "Failed to stat compiled output file");
        return result_enum::CPE;
    }
    if ((long)st.st_size > exec_size_limit)
    {
        unlink(output_run_path.c_str());
        return result_enum::CPE;
    }

    LOG_INFO_USER(user_id, "Compilation completed for submission " + submission_id + " with result " + general_utilities::enum_to_string(result_enum::OK) + " in " + std::to_string(compile_time_ms) + "ms, time_limit: " + std::to_string((long long)time_limit) + "ms");
    return result_enum::OK;
}

