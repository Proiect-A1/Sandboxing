#include <Tasks/stdio_runner_task.h>

bool stdio_runner_task::check_permissions()
{
  //trebuie facut ceva calumea aici
  if (exec_path.empty() || input_path.empty() || output_path.empty())
  {
    return false;
  }

  if (time_limit <= 0 || memory_limit <= 0)
  {
    return false;
  }

  return true;
}

result_enum stdio_runner_task::execute(pthread_t thread_id, int user_id)
{
  (void)thread_id;

  time_consumed = 0;
  memory_consumed = 0;

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

  const std::string exec_file_name = std::filesystem::path(exec_path).filename().string();
  const std::string input_file_name = std::filesystem::path(input_path).filename().string();
  const std::string output_file_name = std::filesystem::path(output_path).filename().string();

  if (exec_file_name.empty() || input_file_name.empty() || output_file_name.empty())
  {
    LOG_ERROR_USER(user_id, "Invalid file paths");
    return result_enum::FAIL;
  }

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

  LOG_INFO_USER(user_id, "Waiting for memory allocation of " + std::to_string(memory_limit) + " B");
  memory_manager &memory = memory_manager::get_instance();
  const unsigned long long requested_memory = memory_limit;
  memory.blocking_request_memory(requested_memory);
  LOG_INFO_USER(user_id, "Memory allocated " + std::to_string(memory_limit) + " B");

  const auto started_at = std::chrono::steady_clock::now();

  pid_t pid = fork();

  if (pid < 0)
  {
    memory.release_memory(requested_memory);
    LOG_ERROR_USER(user_id, "Failed to fork process for execution");
    return result_enum::FAIL;
  }


  if (pid == 0)
  {
    setpgid(0, 0);

    // Trebuie ori reconfigurat runner-u ca sa poata rula si checkere, asta inseamna sa aiba pe langa input si output, sa aiba correct output, si de asemenea sa poata rula ca strong user(marat)
    // ORIIIII, sandboxingu asta sa fie mutat in utilities. Up to cine are chef
    if (chdir(run_dir.c_str()) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to change directory to run directory");
        _exit(127);
    }
    
    if (!(architecture_utilities::change_root_to_sandbox()))
    {
      LOG_ERROR_USER(user_id, "Failed to change root to sandbox");
      _exit(127);
    }

    if (initgroups(run_username.c_str(), pw.pw_gid) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to initialize group access inside sandbox");
      _exit(127);
    }

    if (setgid(pw.pw_gid) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to set group ID inside sandbox");
      _exit(127);
    }

    if (setuid(pw.pw_uid) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to set user ID inside sandbox");
      _exit(127);
    }

    const std::string jailed_input_path = input_file_name;
    const std::string jailed_output_path = output_file_name;

    int in_fd = open(jailed_input_path.c_str(), O_RDONLY);
    if (in_fd < 0)
    {
      LOG_ERROR_USER(user_id, "Failed to open input file inside sandbox");
      _exit(127);
    }
    
    int out_fd = open(jailed_output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
      close(in_fd);
      LOG_ERROR_USER(user_id, "Failed to open output file inside sandbox " + run_username);
      _exit(127);
    }

    if (dup2(in_fd, STDIN_FILENO) < 0 || dup2(out_fd, STDOUT_FILENO) < 0)
    {
      close(in_fd);
      close(out_fd);
      LOG_ERROR_USER(user_id, "Failed to redirect input/output inside sandbox");
      _exit(127);
    }

    close(in_fd);
    close(out_fd);

    struct rlimit memory_rl;
    memory_rl.rlim_cur = (rlim_t)memory_limit;
    memory_rl.rlim_max = (rlim_t)memory_limit;
    if (setrlimit(RLIMIT_AS, &memory_rl) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to set memory limit");
      _exit(127);
    }


    const std::string jailed_exec_path = "./" + exec_file_name;
    char *const argv[] = {const_cast<char *>(jailed_exec_path.c_str()), nullptr};
    execv(jailed_exec_path.c_str(), argv);
    LOG_ERROR_USER(user_id, "Failed to execute the program inside sandbox");

    _exit(127);
  }

  setpgid(pid, pid);

  int status = 0;
  bool timed_out = false;
  struct rusage usage;

  while (true)
  {
    pid_t waited = wait4(pid, &status, WNOHANG, &usage);
    if (waited == pid)
    {
      break;
    }

    if (waited < 0)
    {
      killpg(pid, SIGKILL);
      wait4(pid, &status, 0, &usage);
      memory.release_memory(requested_memory);
      LOG_ERROR_USER(user_id, "Error while waiting for child process");
      return result_enum::FAIL;
    }

    const auto now = std::chrono::steady_clock::now();
    const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - started_at).count();
    if (elapsed_ms > (long long)time_limit)
    {
      timed_out = true;
      killpg(pid, SIGKILL);
      wait4(pid, &status, 0, &usage);
      break;
    }

    usleep(1000);
  }

  const auto ended_at = std::chrono::steady_clock::now();
  time_consumed = (float)std::chrono::duration_cast<std::chrono::milliseconds>(ended_at - started_at).count();
  memory_consumed = (long)(usage.ru_maxrss * 1024L);

  memory.release_memory(requested_memory);

  if (timed_out)
  {
    return result_enum::TLE;
  }

  if (memory_consumed > memory_limit)
  {
    return result_enum::MLE;
  }

  if (WIFEXITED(status))
  {
    const int code = WEXITSTATUS(status);
    if (code == 0)
    {
      return result_enum::OK;
    }
    if (code == 127)
    {
      LOG_ERROR_USER(user_id, std::string("Execution failed inside sandbox"));
      return result_enum::FAIL;
    }
    return result_enum::RTE;
  }

  if (WIFSIGNALED(status))
  {
    const int sig = WTERMSIG(status);
    if (sig == SIGKILL && memory_consumed > memory_limit)
    {
      return result_enum::MLE;
    }
    return result_enum::RTE;
  }

  return result_enum::OK;
}