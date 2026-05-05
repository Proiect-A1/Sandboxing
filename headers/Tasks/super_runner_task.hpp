#ifndef RUNNER_TASK_H
#define RUNNER_TASK_H
#include <string>
#include <Enums/result_enum.h>
#include <Tasks/task.h>
#include <Utilities/general_utilities.h>
#include <Utilities/architecture_utilities.h>
#include <Singletoni/memory_manager.h>

#include <chrono>
#include <cstdlib>
#include <csignal>
#include <filesystem>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <seccomp.h>
#include <errno.h>
#include <fstream>
class super_runner_task : public task
{
  std::string exec_path;
  std::string stdin_redirection_path;
  std::string stdout_redirection_path;
  std::string stderr_redirection_path;
  float time_limit;     // in milliseconds
  long memory_limit;    // in bytes
  float time_consumed;  // in milliseconds
  long memory_consumed; // in bytes
  bool strong_user; // if true, atunci esti marat, if false, atunci esti amarat

  std::vector<std::string> input_files; // fisiere pt care trebuie verificat daca exista in sandbox si daca se pot citi;
  std::vector<std::string> output_files; // fisiere pt care trebuie verificat daca exista in sandbox si daca se pot scrie;
  std::vector<std::string> arguments; // argumente cu care sa fie rulat programul, inclusiv numele executabilului
public:
  super_runner_task(std::string exec_path, std::string stdin_redirection_path, std::string stdout_redirection_path, std::string stderr_redirection_path, float time_limit, long memory_limit, std::vector<std::string> input_files, std::vector<std::string> output_files, std::vector<std::string> arguments, bool strong_user = false)
      : exec_path(exec_path), stdin_redirection_path(stdin_redirection_path), stdout_redirection_path(stdout_redirection_path), stderr_redirection_path(stderr_redirection_path), time_limit(time_limit), memory_limit(memory_limit), time_consumed(0), memory_consumed(0), input_files(input_files), output_files(output_files), arguments(arguments), strong_user(strong_user) {}

  bool check_permissions(int user_id) override;
  result_enum execute(pthread_t thread_id, int user_id) override;

  float get_time_consumed() const { return time_consumed; }
  long get_memory_consumed() const { return memory_consumed; }
};

#endif