#include <stdio_runner_task.h>

result_enum stdio_runner_task::execute(){
  char command[1024];
  snprintf(command, sizeof(command), "%s < %s > %s", exec_path.c_str(), input_path.c_str(), output_path.c_str());
  return (system(command) == 0) ? OK : RTE;
}