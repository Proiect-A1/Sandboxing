#include <Utilities/architecture_utilities.h>


bool architecture_utilities::change_root_to_user(std::string username){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path) + "/runs/" + username;
  chroot(user_dir.c_str());
  return true;

}

bool architecture_utilities::change_root_to_sandbox(){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path);
  if (chroot(user_dir.c_str()) != 0)
  {
    return false;
  }
  return true;
}
bool architecture_utilities::change_dir_to_user(std::string username){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path) + "/runs/" + username;
  chdir(user_dir.c_str());
  return true;

}

bool architecture_utilities::change_dir_to_sandbox(){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path);
  chdir(user_dir.c_str());
  return true;
}




std::string architecture_utilities::get_run_dir(std::string username){
  return std::string(getenv("SANDBOX_PATH")) + "/runs/" + username;
}

std::string architecture_utilities::get_submission_dir(std::string submission_id){
  return std::string(getenv("SANDBOX_PATH")) + "/submissions/" + submission_id;
}

std::string architecture_utilities::get_submission_source_path(std::string submission_id, language_enum language) {
  if (language == language_enum::CPP) {
    return std::string(getenv("SANDBOX_PATH")) + "/submissions/" + submission_id + "/main.cpp";
  }
  return "";
}

std::string architecture_utilities::get_submission_exec_path(std::string submission_id, language_enum language) {
  if (language == language_enum::CPP) {
    return std::string(getenv("SANDBOX_PATH")) + "/submissions/" + submission_id + "/main_exec";
  }
  return "";
}
std::string architecture_utilities::get_problem_input_path(std::string problem_id, int rev_id, int test) {
  return std::string(getenv("SANDBOX_PATH")) + "/inputs/" + problem_id + "." + std::to_string(rev_id) + "/" + general_utilities::left_zero_pad(test, 3) + ".in";
}
std::string architecture_utilities::get_problem_correct_output_path(std::string problem_id, int rev_id, int test) {
  return std::string(getenv("SANDBOX_PATH")) + "/correct_outputs/" + problem_id + "." + std::to_string(rev_id) + "/" + general_utilities::left_zero_pad(test, 3) + ".ok";
}