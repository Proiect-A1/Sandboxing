#ifndef RUNNER_TASK_H
#define RUNNER_TASK_H
#include <string>
#include <result_enum.h>
class stdio_runner_task{
  std::string exec_path;
  std::string input_path;
  std::string output_path;
  float time_limit; // in milliseconds
  long memory_limit; // in bytes
  public:
  stdio_runner_task(std::string exec_path, std::string input_path, std::string output_path, float time_limit, long memory_limit) : 
    exec_path(exec_path), input_path(input_path), output_path(output_path), time_limit(time_limit), memory_limit(memory_limit) {}
  result_enum execute();

};

#endif