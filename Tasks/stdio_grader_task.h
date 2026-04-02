#ifndef STDIO_GRADER_TASK_H
#define STDIO_GRADER_TASK_H
#include <result_enum.h>
#include <problem_metadata.h>
#include <submission_test.h>
#include <string>
class stdio_grader_task{
  submission_test* test; // the test that needs to be executed, this structure is used for passing data back to the user
  std::string main_exec_path; 
  std::string input_path;
  std::string checker_path;
  std::string correct_output_path;
  float time_limit; // in milliseconds
  long memory_limit; // in bytes
  // std::string interactor_exec_path;

  public:
  result_enum execute();
  stdio_grader_task(submission_test* test, std::string main_exec_path, std::string input_path, std::string checker_path, std::string correct_output_path, float time_limit, long memory_limit) : 
    test(test), main_exec_path(main_exec_path), input_path(input_path), checker_path(checker_path), correct_output_path(correct_output_path), time_limit(time_limit), memory_limit(memory_limit) {}
};

#endif