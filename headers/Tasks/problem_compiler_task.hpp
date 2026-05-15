#ifndef PROBLEM_COMPILER_TASK_HPP
#define PROBLEM_COMPILER_TASK_HPP
#include <string>
#include <Singletoni/logger.h>
#include <Tasks/task.h>
#include <Tasks/stdio_compiler_task.h>
#include <problem_manager.h>
class problem_compiler_task : public task {
  std::string problem_id;
  int rev_id;
  stdio_compiler_task* compiler_task;
public:
  problem_compiler_task(const std::string& problem_id, int rev_id, stdio_compiler_task* compiler_task);
  ~problem_compiler_task(){
    delete compiler_task;
  }
  bool check_permissions(int user_id) override {
    return true;
  }
  result_enum execute(pthread_t thread_id, int user_id) override;
};






#endif