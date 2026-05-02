#include <Tasks/validator_task.hpp>

result_enum validator_task::execute(pthread_t thread_id, int user_id){
  std::string run_dir = architecture_utilities::get_run_dir(user_id);
  std::string input_file_path = run_dir + "/" + input_path;

  //comentat este o varianta fara jail, astept jailingul avansat ca sa pot rula scriptul de validare in sandbox
  //deocamdata totu e bine
  // if (system((validator_script_path + " " + input_file_path).c_str()) != 0) {
  //   LOG_ERROR_USER(user_id, "Validator script execution failed");
  //   return result_enum::FAIL;
  // }

  // LOG_INFO_USER(user_id, "Validator script executed successfully");
  return result_enum::OK;
}