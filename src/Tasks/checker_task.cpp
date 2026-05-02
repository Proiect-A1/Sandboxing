#include <Tasks/checker_task.h>


result_enum checker_task::execute(pthread_t thread_id, int user_id) {
  if (system(("diff -q " + architecture_utilities::get_run_dir(user_id) + "/" + output + " " + architecture_utilities::get_run_dir(user_id) + "/" + correct_output).c_str()) != 0)
  {
    point_percentage = 0;
    message = "WA!";
    return result_enum::OK;
  }

  point_percentage = 1;
  message = "OK!";
  LOG_INFO_USER(user_id, "Checker task finished");
  return result_enum::OK;

}
