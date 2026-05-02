#ifndef EVALUATOR_TASK_H
#define EVALUATOR_TASK_H

#include <Tasks/stdio_compiler_factory.h>
#include <Tasks/stdio_grader_task.h>
#include <Tasks/stdio_runner_factory.h>
#include <Singletoni/memory_manager.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <vector>
#include <map>
#include <iostream>
#include <Singletoni/task_queue.h>
class evaluator_task : public task {
  std::string submission_id;
  std::string problem_id;
  int rev_id;

  public:
  evaluator_task(std::string submission_id, std::string problem_id, int rev_id) : submission_id(submission_id), problem_id(problem_id), rev_id(rev_id) {}
  result_enum execute(pthread_t thread_id, int user_id) override;
  bool check_permissions() override { return true; };
};


#endif // EVALUATOR_TASK_H