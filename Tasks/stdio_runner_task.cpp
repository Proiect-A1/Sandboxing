#include <stdio_runner_task.h>

#include <Singletoni/memory_manager.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

bool stdio_runner_task::check_permissions()
{
  if (exec_path.empty() || input_path.empty() || output_path.empty())
  {
    return false;
  }

  if (time_limit <= 0 || memory_limit <= 0)
  {
    return false;
  }

  return true;
}

result_enum stdio_runner_task::execute()
{
  if (!check_permissions())
  {
    return FAIL;
  }

  memory_manager &memory = memory_manager::get_instance();
  const unsigned long long requested_memory = memory_limit;
  memory.blocking_request_memory(requested_memory);

  char command[1024];
  const unsigned long long memory_limit_kb = (requested_memory + 1023ULL) / 1024ULL;
  const unsigned long long timeout_ms = (unsigned long long)time_limit;
  const double timeout_seconds = timeout_ms / 1000.0;
  snprintf(
      command,
      sizeof(command),
      "bash -lc \"ulimit -v %llu; timeout %.3fs '%s' < '%s' > '%s'\"",
      memory_limit_kb,
      timeout_seconds,
      exec_path.c_str(),
      input_path.c_str(),
      output_path.c_str());

  const int status = system(command);
  memory.release_memory(requested_memory);

  if (status == -1)
  {
    return FAIL;
  }

  if (WIFEXITED(status))
  {
    const int code = WEXITSTATUS(status);
    if (code == 0)
    {
      return OK;
    }
    if (code == 124)
    {
      return TLE;
    }
    if (code == 137)
    {
      return MLE;
    }
    if (code == 125 || code == 126 || code == 127)
    {
      return FAIL;
    }
    return RTE;
  }

  if (WIFSIGNALED(status))
  {
    return RTE;
  }

  return FAIL;
}