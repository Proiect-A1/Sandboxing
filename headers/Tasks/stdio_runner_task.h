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
class stdio_runner_task : public task
{
  std::string exec_path;
  std::string input_path;
  std::string output_path;
  float time_limit;     // in milliseconds
  long memory_limit;    // in bytes
  float time_consumed;  // in milliseconds
  long memory_consumed; // in bytes

public:
  stdio_runner_task(std::string exec_path, std::string input_path, std::string output_path, float time_limit, long memory_limit)
      : exec_path(exec_path), input_path(input_path), output_path(output_path), time_limit(time_limit), memory_limit(memory_limit), time_consumed(0), memory_consumed(0) {}

  bool check_permissions() override;
  result_enum execute(pthread_t thread_id, int user_id) override;

  float get_time_consumed() const { return time_consumed; }
  long get_memory_consumed() const { return memory_consumed; }
  void print_error(pthread_t thread_id, int user_id, const std::string& message) override;
  void print_log(pthread_t thread_id, int user_id, const std::string& message) override;
};

#endif