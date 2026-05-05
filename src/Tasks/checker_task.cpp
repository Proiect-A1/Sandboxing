#include <Tasks/checker_task.h>


result_enum checker_task::execute(pthread_t thread_id, int user_id) {
  // if (system(("diff -q " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + output + " " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + correct_output).c_str()) != 0)
  // {
  //   point_percentage = 0;
  //   message = "not the same";
  //   return result_enum::OK;
  // }

  auto checker = runner_factories::checker_runner_factory[language_enum::CPP](
    submission_id;
  );

  point_percentage = 1;
  message = "OK!";
  LOG_INFO_USER(user_id, "Checker task finished");
  return result_enum::OK;

}
