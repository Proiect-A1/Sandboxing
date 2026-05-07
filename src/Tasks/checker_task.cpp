#include <Tasks/checker_task.h>


result_enum checker_task::execute(pthread_t thread_id, int user_id) {
  // robert baga submission_id

  if (system(("diff -wBb -q " + output + " " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + correct_output).c_str()) != 0)
  {
    point_percentage = 0;
    message = "not the same";
    std::string str =  "Output: " + general_utilities::syscall_to_string("cat " + output) + "\n_____________________";
    LOG_DEBUG_USER(user_id, str);
    str = "Correct output: " + general_utilities::syscall_to_string("cat " + architecture_utilities::get_run_dir_absolute_path(user_id) + "/" + correct_output) + "\n_____________________";
    LOG_DEBUG_USER(user_id, str);
    return result_enum::OK;
  }

  point_percentage = 1;
  message = "OK!";
  LOG_INFO_USER(user_id, "Checker task finished");
  return result_enum::OK;


  std::string run_dir = architecture_utilities::get_run_dir_absolute_path(user_id);
  std::string checker_output_path = run_dir + "/" + "checker_output";
  std::string checker_message_path = run_dir + "/" + "checker_message";

  auto checker = runner_factories::checker_runner_factory[language_enum::COMPILED](
    submission_id,
    contestant_source,
    checker_output_path,
    checker_message_path,
    input,
    output,
    correct_output,
    contestant_source,
    arguments,
    0);

  result_enum aux_rez = checker->execute(thread_id, user_id);

  if (aux_rez != result_enum::OK && aux_rez != result_enum::RTE){
    LOG_ERROR_USER(user_id, "Checker finished with NON-OK result: " + general_utilities::enum_to_string(aux_rez));
    return aux_rez;
  }
  if (aux_rez == result_enum::RTE){
    switch(checker->get_exit_code()){
      case 1:
      break;


      default:
      return result_enum::RTE;
    }
  }
  point_percentage = 1;
  return result_enum::OK;
}
