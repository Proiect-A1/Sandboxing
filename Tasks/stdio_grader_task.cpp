#include <stdio_grader_task.h>
#include <stdio_runner_task.h>
result_enum stdio_grader_task::execute()
{
  char command[1024];
  snprintf(command, sizeof(command), "cp %s main", main_exec_path.c_str());
  if (system(command) != 0)
  {
    test->result = FAIL;
    test->points = 0;
    return FAIL;
  }
  snprintf(command, sizeof(command), "cp %s input.txt", input_path.c_str());
  if (system(command) != 0)
  {
    test->result = FAIL;
    test->points = 0;
    return FAIL;
  }
  stdio_runner_task runner(main_exec_path, "input.txt", "output.txt", time_limit, memory_limit);
  result_enum runner_result = runner.execute(0, 0);
  if (runner_result != OK)
  {
    test->result = runner_result;
    test->points = 0;
    snprintf(command, sizeof(command), "rm * -r");
    system(command);
    return runner_result;
  }
  snprintf(command, sizeof(command), "cp %s correct.txt", correct_output_path.c_str());
  if (system(command) != 0)
  {
    test->result = FAIL;
    test->points = 0;
    return FAIL;
  }
  snprintf(command, sizeof(command), "%s input.txt output.txt correct.txt", checker_path.c_str());
  int checker_result = system(command);
  if (checker_result == 0)
  {
    test->result = OK;
    test->points = 100;
  }
  else
  {
    test->result = WA;
    test->points = 0; // this is a placeholder, the actual points should be determined by the checker
  }
  snprintf(command, sizeof(command), "rm -r");
  system(command);
  return test->result;
}
