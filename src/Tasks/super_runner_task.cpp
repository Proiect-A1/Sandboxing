#include <Tasks/super_runner_task.hpp>

static int install_seccomp_whitelist()
{
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_TRAP);
  if (!ctx) return -1;

  auto add = [&](int sys) {
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, sys, 0) < 0) return false;
    return true;
  };

  int syscalls[] = {
    SCMP_SYS(read), SCMP_SYS(write), SCMP_SYS(writev),
    SCMP_SYS(open), SCMP_SYS(openat), SCMP_SYS(close), SCMP_SYS(lseek),
    SCMP_SYS(fstat), SCMP_SYS(newfstatat), SCMP_SYS(stat), SCMP_SYS(pread64),
    SCMP_SYS(access), SCMP_SYS(readlink),
    SCMP_SYS(mmap), SCMP_SYS(munmap), SCMP_SYS(mprotect), SCMP_SYS(brk), SCMP_SYS(madvise),
    SCMP_SYS(arch_prctl), SCMP_SYS(set_tid_address), SCMP_SYS(set_robust_list),
    SCMP_SYS(prlimit64), SCMP_SYS(getrandom), SCMP_SYS(rseq),
    SCMP_SYS(futex), SCMP_SYS(ioctl), SCMP_SYS(fcntl), SCMP_SYS(uname), SCMP_SYS(sysinfo),
    SCMP_SYS(rt_sigaction), SCMP_SYS(rt_sigprocmask), SCMP_SYS(rt_sigreturn),
    SCMP_SYS(execve), SCMP_SYS(exit), SCMP_SYS(exit_group)
  };

  for (int s : syscalls)
  {
    if (!add(s))
    {
      seccomp_release(ctx);
      return -1;
    }
  }

  if (seccomp_load(ctx) < 0)
  {
    seccomp_release(ctx);
    return -1;
  }
  seccomp_release(ctx);
  return 0;
}

bool super_runner_task::check_permissions(int user_id)
{
  // internal consistency checks
  if (time_limit <= 0 || memory_limit <= 0)
  {
    LOG_ERROR_USER(user_id, "Invalid time or memory limit in super_runner_task");
    return false;
  }
  if (exec_path.empty()){
    LOG_ERROR_USER(user_id, "Executable path is empty in super_runner_task");
    return false;
  }
  if (stdin_redirection_path.empty()){
    LOG_ERROR_USER(user_id, "Stdin redirection path is empty in super_runner_task");
    return false;
  }
  if (stdout_redirection_path.empty()){
    LOG_ERROR_USER(user_id, "Stdout redirection path is empty in super_runner_task");
    return false;
  }
  if (stderr_redirection_path.empty()){
    LOG_ERROR_USER(user_id, "Stderr redirection path is empty in super_runner_task");
    return false;
  }

  for (const std::string& input_file : input_files) {
    if (input_file.empty()) {
      LOG_ERROR_USER(user_id, "One of the input files has an empty path in super_runner_task");
      return false;
    }
  }
  // de avut o discutie legata de necesitatea acestei bucati
  for (const std::string& output_file : output_files) {
    if (output_file.empty()) {
      LOG_ERROR_USER(user_id, "One of the output files has an empty path in super_runner_task");
      return false;
    }
  }
  for (const std::string& arg : arguments) {
    if (arg.empty()) {
      LOG_ERROR_USER(user_id, "One of the arguments is an empty string in super_runner_task");
      return false;
    }
  }

  // context checks
  for (const std::string& input_file : input_files) {
    if (!std::filesystem::exists(input_file)) {
      LOG_ERROR_USER(user_id, "Input file does not exist: " + input_file);
      return false;
    }
    if (!std::filesystem::is_regular_file(input_file)) {
      LOG_ERROR_USER(user_id, "Input file is not a regular file: " + input_file);
      return false;
    }
    if (access(input_file.c_str(), R_OK) != 0) {
      LOG_ERROR_USER(user_id, "Input file is not readable: " + input_file);
      return false;
    }
  }

  for (const std::string& output_file : output_files) {
    if (std::filesystem::exists(output_file)) {
      if (!std::filesystem::is_regular_file(output_file)) {
        LOG_ERROR_USER(user_id, "Output file exists but is not a regular file: " + output_file);
        return false;
      }
      if (access(output_file.c_str(), W_OK) != 0) {
        LOG_ERROR_USER(user_id, "Output file exists but is not writable: " + output_file);
        return false;
      }
    } else {
      // check if we can create the file
      std::ofstream ofs(output_file);
      if (!ofs) {
        LOG_ERROR_USER(user_id, "Output file does not exist and cannot be created: " + output_file);
        return false;
      }
      ofs.close();
      std::filesystem::remove(output_file);
    }
  }


  return true;
}

result_enum super_runner_task::execute(pthread_t thread_id, int user_id)
{
  (void)thread_id;

  time_consumed = 0;
  memory_consumed = 0;

  if (!check_permissions(user_id))
  {
    LOG_ERROR_USER(user_id, "Permission check failed BEFORE SANDBOXING");
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

  const std::string run_username = (strong_user) ? architecture_utilities::get_strong_user(user_id) : architecture_utilities::get_weak_user(user_id);
  const std::string run_dir = architecture_utilities::get_run_dir_absolute_path(user_id);

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
    
    // DECOMENTATI TOT CE TINE DE err_fd CA SA DATI REDIRECT STDERR-ului LA /dev/null 
    // all file descriptors are set before sandboxing as they are kept open
    // trebuie inchisi toti fd in afara de stdin stdout stderr inainte de a da exec ca sa nu poata face prostii
    // robert tine minte sa te joci cu fd-urile sa vezi daca mai trebuie copiate chestiile sau nu
    int err_fd = open(stderr_redirection_path.c_str(), O_WRONLY);
    if (err_fd < 0)
    {
      LOG_ERROR_USER(user_id, "Failed to open stderr_redirection_path before sandbox restrictions: " + stderr_redirection_path);
      _exit(127);
    }
    // Trebuie ori reconfigurat runner-u ca sa poata rula si checkere, asta inseamna sa aiba pe langa input si output, sa aiba correct output, si de asemenea sa poata rula ca strong user(marat)
    // ORIIIII, sandboxingu asta sa fie mutat in utilities. Up to cine are chef
    
    if (initgroups(run_username.c_str(), pw.pw_gid) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to initialize group access inside sandbox");
      _exit(127);
    }
    
    if (!(architecture_utilities::change_root_to_sandbox()))
    {
      LOG_ERROR_USER(user_id, "Failed to change root to sandbox");
      _exit(127);
    }
    
    std::string inner_run_dir = architecture_utilities::get_run_dir_relative_to_sandbox_path(user_id);
    if (chdir(inner_run_dir.c_str()) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to change directory to run directory inside sandbox");
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
    
    
    int in_fd = open(stdin_redirection_path.c_str(), O_RDONLY);
    if (in_fd < 0)
    {
      LOG_ERROR_USER(user_id, "Failed to open input file inside sandbox " + run_username + " " + stdin_redirection_path + " " + general_utilities::syscall_to_string("ls") + general_utilities::syscall_to_string("whoami"));
      _exit(127);
    }
    
    int out_fd = open(stdout_redirection_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
      close(in_fd);
      LOG_ERROR_USER(user_id, "Failed to open output file inside sandbox " + run_username + " " + stdout_redirection_path + " " + general_utilities::syscall_to_string("ls") + general_utilities::syscall_to_string("whoami"));
      _exit(127);
    }
    
    if (dup2(in_fd, STDIN_FILENO) < 0 || dup2(out_fd, STDOUT_FILENO) < 0 || dup2(err_fd, STDERR_FILENO) < 0)
    {
      close(in_fd);
      close(out_fd);
      close(err_fd);
      LOG_ERROR_USER(user_id, "Failed to redirect input/output/error inside sandbox");
      _exit(127);
    }
    
    close(in_fd);
    close(out_fd);
    close(err_fd);

    if (!check_permissions(user_id)){
      LOG_ERROR_USER(user_id, "Permission check failed AFTER SANDBOXING");
      _exit(127);
    }
    
    struct rlimit memory_rl;
    rlim_t mem_limit_padded = (rlim_t)memory_limit + 64 * 1024 * 1024;
    memory_rl.rlim_cur = mem_limit_padded;
    memory_rl.rlim_max = mem_limit_padded;
    if (setrlimit(RLIMIT_AS, &memory_rl) != 0){
      LOG_ERROR_USER(user_id, "Failed to set memory limit");
      _exit(127);
    }

    long sec = (long)std::ceil(time_limit / 1000.0f);
    if (sec <= 0) sec = 1;
    struct rlimit cpu_rl;
    cpu_rl.rlim_cur = (rlim_t)sec;
    cpu_rl.rlim_max = (rlim_t)sec + 1;
    if (setrlimit(RLIMIT_CPU, &cpu_rl) != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to set CPU time limit");
      _exit(127);
    }
    // LOG_INFO_USER(user_id, "WOHOOO SUNT SMECHER" + general_utilities::syscall_to_string("whoami"));

    if (install_seccomp_whitelist() != 0)
    {
      LOG_ERROR_USER(user_id, "Failed to install seccomp filter");
      _exit(127);
    }

    const char* argv[arguments.size() + 1];
    for (size_t i = 0; i < arguments.size(); ++i)
    {
        argv[i] = arguments[i].c_str();
    }
    argv[arguments.size()] = nullptr;

    execv(exec_path.c_str(), const_cast<char *const *>(argv));
    LOG_ERROR_USER(user_id, "Failed to execute the program inside sandbox");

    _exit(127);
  }

  setpgid(pid, pid);

  int status = 0;
  struct rusage usage;

  auto start_time = std::chrono::steady_clock::now();
  bool time_limit_exceeded = false;

  while (true)
  {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
    
    if (elapsed_ms > time_limit + 1000)
    {
      time_limit_exceeded = true;
      killpg(pid, SIGKILL);
    }

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

    usleep(1000);
  }

  long long used_us = (long long)usage.ru_utime.tv_sec * 1000000LL + (long long)usage.ru_utime.tv_usec;
  time_consumed = (float)(used_us / 1000.0f);
  memory_consumed = (long)(usage.ru_maxrss * 1024L);

  memory.release_memory(requested_memory);

  exit_code = status;

  if (time_limit_exceeded)
  {
    return result_enum::TLE;
  }

  if (memory_consumed > memory_limit)
  {
    return result_enum::MLE;
  }

  if (time_consumed > time_limit)
  {
    return result_enum::TLE;
  }

  if (WIFSIGNALED(status))
  {
    const int sig = WTERMSIG(status);
      
    if (sig == SIGXCPU)
    {
      return result_enum::TLE;
    }

    if (sig == SIGSYS)
    {
      LOG_OTHER_USER(user_id, std::string("Seccomp violation: child pid=") + std::to_string(pid) + std::string(" exec=") + exec_path);
      return result_enum::RTE;
    }
      
    if ((sig == SIGABRT || sig == SIGSEGV || sig == SIGKILL) && memory_consumed > memory_limit)
    {
      return result_enum::MLE;
    }

    if (sig == SIGKILL && time_consumed > time_limit)
    {
      return result_enum::TLE;
    }
    
    return result_enum::RTE;
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

  return result_enum::RTE;
}