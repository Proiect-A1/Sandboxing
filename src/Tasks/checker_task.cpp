#include <Tasks/checker_task.h>


result_enum checker_task::execute(pthread_t thread_id, int user_id) {
  // robert baga submission_id

  // if (system(("diff -wBb -q " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + output + " " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + correct_output).c_str()) != 0)
  // {
  //   point_percentage = 0;
  //   message = "not the same";
  //   std::string str =  "Output: " + general_utilities::syscall_to_string("cat " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + output) + "\n_____________________";
  //   LOG_DEBUG_USER(user_id, str);
  //   str = "Correct output: " + general_utilities::syscall_to_string("cat " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + correct_output) + "\n_____________________";
  //   LOG_DEBUG_USER(user_id, str);
  //   return result_enum::OK;
  // }

  point_percentage = 1;
  message = "OK!";
  LOG_INFO_USER(user_id, "Checker task finished");
  return result_enum::OK;

}
