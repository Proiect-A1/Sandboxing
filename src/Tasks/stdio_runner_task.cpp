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

result_enum stdio_runner_task::execute(int thread_id, int user_id)
{
  (void)thread_id;

  time_consumed = 0;
  memory_consumed = 0;

  if (!check_permissions())
  {
    print_error(thread_id, user_id, "Permission check failed");
    return result_enum::FAIL;
  }

  if (user_id <= 0)
  {
    print_error(thread_id, user_id, "Invalid user ID");
    return result_enum::FAIL;
  }

  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    print_error(thread_id, user_id, "Sandbox path is not set in environment variables");
    return result_enum::FAIL;
  }

  const std::string run_username = "amarat" + std::to_string(user_id);
  const std::string run_dir = submission_info_utilities::get_run_dir(run_username);

  const std::string exec_file_name = std::filesystem::path(exec_path).filename().string();
  const std::string input_file_name = std::filesystem::path(input_path).filename().string();
  const std::string output_file_name = std::filesystem::path(output_path).filename().string();

  if (exec_file_name.empty() || input_file_name.empty() || output_file_name.empty())
  {
    print_error(thread_id, user_id, "Invalid file paths");
    return result_enum::FAIL;
  }


  struct passwd *pw = getpwnam(run_username.c_str());
  if (pw == nullptr)
  {
    print_error(thread_id, user_id, "Failed to get user information for sandbox user");
    return result_enum::FAIL;
  }

  memory_manager &memory = memory_manager::get_instance();
  const unsigned long long requested_memory = memory_limit;
  memory.blocking_request_memory(requested_memory);

  const auto started_at = std::chrono::steady_clock::now();
  pid_t pid = fork();

  if (pid < 0)
  {
    memory.release_memory(requested_memory);
    return result_enum::FAIL;
  }

  if (pid == 0)
  {
    setpgid(0, 0);

    if (chdir(run_dir.c_str()) != 0)
    {
      print_error(thread_id, user_id, "Failed to change directory to run directory");
        _exit(127);
    }
    
    if (!(utilities::change_root_to_sandbox()))
    {
      print_error(thread_id, user_id, "Failed to change root to sandbox");
      _exit(127);
    }

    if (initgroups(run_username.c_str(), pw->pw_gid) != 0)
    {
      print_error(thread_id, user_id, "Failed to initialize group access inside sandbox");
      _exit(127);
    }

    if (setgid(pw->pw_gid) != 0)
    {
      print_error(thread_id, user_id, "Failed to set group ID inside sandbox");
      _exit(127);
    }

    if (setuid(pw->pw_uid) != 0)
    {
      print_error(thread_id, user_id, "Failed to set user ID inside sandbox");
      _exit(127);
    }

    const std::string jailed_input_path = input_file_name;
    const std::string jailed_output_path = output_file_name;

    int in_fd = open(jailed_input_path.c_str(), O_RDONLY);
    if (in_fd < 0)
    {
      print_error(thread_id, user_id, "Failed to open input file inside sandbox");
      _exit(127);
    }

    int out_fd = open(jailed_output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
      close(in_fd);
      print_error(thread_id, user_id, "Failed to open output file inside sandbox");
      _exit(127);
    }

    if (dup2(in_fd, STDIN_FILENO) < 0 || dup2(out_fd, STDOUT_FILENO) < 0)
    {
      close(in_fd);
      close(out_fd);
      print_error(thread_id, user_id, "Failed to redirect input/output inside sandbox");
      _exit(127);
    }

    close(in_fd);
    close(out_fd);

    struct rlimit memory_rl;
    memory_rl.rlim_cur = (rlim_t)memory_limit;
    memory_rl.rlim_max = (rlim_t)memory_limit;
    if (setrlimit(RLIMIT_AS, &memory_rl) != 0)
    {
      print_error(thread_id, user_id, "Failed to set memory limit");
      _exit(127);
    }

    const std::string jailed_exec_path = "./" + exec_file_name;
    char *const argv[] = {const_cast<char *>(jailed_exec_path.c_str()), nullptr};
    execv(jailed_exec_path.c_str(), argv);
    print_error(thread_id, user_id, "Failed to execute the program inside sandbox");
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
      print_error(thread_id, user_id, "Error while waiting for child process");
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
      print_error(thread_id, user_id, "Execution failed inside sandbox");
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
void stdio_runner_task:: print_error(int thread_id, int user_id,const std::string& message){
  fprintf(stderr, "STDIO_Runner task running on thread %d, with user %d: %s\n", thread_id, user_id, message.c_str());
}