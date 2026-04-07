#include <Utilities/submission_info_utilities.h>


std::string submission_info_utilities::left_zero_pad (int number, int width) {
  std::string num_str = std::to_string(number);
  if (num_str.length() >= width) {
    return num_str;
  }
  return std::string(width - num_str.length(), '0') + num_str;
}

std::string submission_info_utilities::get_run_dir(std::string username){
  return std::string(getenv("SANDBOX_PATH")) + "/runs/" + username;
}

std::string submission_info_utilities::get_submission_dir(std::string submission_id){
  return std::string(getenv("SANDBOX_PATH")) + "/submissions/" + submission_id;
}

std::string submission_info_utilities::get_submission_source_path(std::string submission_id, language_enum language) {
  if (language == language_enum::CPP) {
    return std::string(getenv("SANDBOX_PATH")) + "/submissions/" + submission_id + "/main.cpp";
  }
}

std::string submission_info_utilities::get_submission_exec_path(std::string submission_id, language_enum language) {
  if (language == language_enum::CPP) {
    return std::string(getenv("SANDBOX_PATH")) + "/submissions/" + submission_id + "/main_exec";
  }
}
std::string submission_info_utilities::get_problem_input_path(std::string problem_id, int rev_id, int test) {
  return std::string(getenv("SANDBOX_PATH")) + "/inputs/" + problem_id + "." + std::to_string(rev_id) + "/" + left_zero_pad(test, 3) + ".in";
}
std::string submission_info_utilities::get_problem_correct_output_path(std::string problem_id, int rev_id, int test) {
  return std::string(getenv("SANDBOX_PATH")) + "/correct_outputs/" + problem_id + "." + std::to_string(rev_id) + "/" + left_zero_pad(test, 3) + ".ok";
}