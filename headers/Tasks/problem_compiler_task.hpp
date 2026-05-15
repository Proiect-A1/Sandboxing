#ifndef PROBLEM_COMPILER_TASK_HPP
#define PROBLEM_COMPILER_TASK_HPP
#include <string>
#include <Singletoni/logger.h>
#include <Tasks/task.h>
#include <Tasks/stdio_compiler_task.h>
#include <Tasks/stdio_compiler_factory.h>
#include <filesystem>
#include <Singletoni/problem_manager.h>
class problem_compiler_task : public task {
  std::string problem_id;
  int rev_id;
  std::string source_path;
public:
  problem_compiler_task(const std::string& problem_id, int rev_id, const std::string& source_path): problem_id(problem_id), rev_id(rev_id), source_path(source_path) {
    priority = 3; // Prioritate mai mare pentru compilarea surselor
  }
  bool check_permissions(int user_id) override {
    return true;
  }
  result_enum execute(pthread_t thread_id, int user_id) override;
};






#endif