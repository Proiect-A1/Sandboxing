#ifndef STDIO_GRADER_TASK_H
#define STDIO_GRADER_TASK_H
#include <Tasks/task.h>
#include <Chestii_cu_data/problem_metadata.h>
#include <Chestii_cu_data/submission_test.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <Tasks/stdio_runner_task.h>
#include <Tasks/stdio_runner_factory.h>
#include <Tasks/checker_task.h>
#include <Utilities/architecture_utilities.h>
#include <Utilities/general_utilities.h>
#include <string>
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
class stdio_grader_task : public task{
  struct stdio_grader_helper{
    std::string submission_id;
    result_enum test_result;

    pthread_t thread_id;
    int user_id;
    int test_id;

    submission_test test;
    stdio_grader_helper(result_enum result, std::string sub_id) : test_result(result), submission_id(sub_id) {}
    ~stdio_grader_helper();
  };
  stdio_grader_helper helper;
  std::string submission_id;
  std::string problem_id;
  int rev_id;
  int test_id;

  public :
    stdio_grader_task(std::string submission_id, std::string problem_id, int rev_id, int test) : helper(result_enum::FAIL, submission_id), submission_id(submission_id), problem_id(problem_id), rev_id(rev_id), test_id(test) {}
  result_enum execute(pthread_t thread_id, int user_id) override;
  bool check_permissions() override {return true;};
};

#endif