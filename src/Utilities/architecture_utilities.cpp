#include <Utilities/architecture_utilities.h>


bool architecture_utilities::change_root_to_user(const int& user_id){
  const std::string user_dir = get_run_dir(user_id);
  if (chroot(user_dir.c_str()) != 0){
    return false;
  }
  return true;
}

bool architecture_utilities::change_root_to_sandbox(){
  std::string path = architecture_utilities::get_sandbox_path();
  const char *sandbox_path = path.c_str(); //v1

  if (sandbox_path == nullptr || sandbox_path[0] == '\0'){
    return false;
  }
  if (chroot(sandbox_path) != 0){
    return false;
  }
  return true;
}

bool architecture_utilities::change_dir_to_user(const int& user_id){
  const std::string user_dir = get_run_dir(user_id);
  if (chdir(user_dir.c_str()) != 0){
    return false;
  }
  return true;

}

bool architecture_utilities::change_dir_to_sandbox(){
  std::string path = architecture_utilities::get_sandbox_path();
  const char *sandbox_path = path.c_str(); //v1

  if (sandbox_path == nullptr || sandbox_path[0] == '\0'){
    return false;
  }
  if (chdir(sandbox_path) != 0){
    return false;
  }
  return true;
}


std::string architecture_utilities::get_weak_user(const int& user_id){
  return "amarat" + std::to_string(user_id);
}
std::string architecture_utilities::get_strong_user(const int& user_id){
  return "marat" + std::to_string(user_id);  
}

std::string architecture_utilities::get_run_dir(const int& user_id){
  return get_sandbox_path()+ "/runs/" + get_weak_user(user_id);
}

std::string architecture_utilities::get_submission_dir(std::string submission_id){
  return get_sandbox_path() + "/submissions/" + submission_id;
}

std::string architecture_utilities::get_sandbox_path(){
  return std::string(getenv("SANDBOX_PATH"));
}

std::string architecture_utilities::get_submission_source_path(std::string submission_id, language_enum language) {
  if (language == language_enum::CPP) {
    return get_sandbox_path() + "/submissions/" + submission_id + "/main.cpp";
  }
  return "";
}

std::string architecture_utilities::get_submission_exec_path(std::string submission_id, language_enum language) {
  if (language == language_enum::CPP) {
    return get_sandbox_path() + "/submissions/" + submission_id + "/main_exec";
  }
  return "";
}
std::string architecture_utilities::get_problem_input_path(std::string problem_id, int rev_id, int test) {
  return get_sandbox_path() + "/inputs/" + problem_id + "." + std::to_string(rev_id) + "/" + general_utilities::left_zero_pad(test, 3) + ".in";
}
std::string architecture_utilities::get_problem_correct_output_path(std::string problem_id, int rev_id, int test) {
  return get_sandbox_path() + "/correct_outputs/" + problem_id + "." + std::to_string(rev_id) + "/" + general_utilities::left_zero_pad(test, 3) + ".ok";
}

std::string architecture_utilities::get_problem_data_folder(const std::string& problem_id, int rev_id){
  return get_sandbox_path() + "/problem_data/" + problem_id + "." + std::to_string(rev_id);
}

std::string architecture_utilities::get_problem_script_folder(const std::string& problem_id, int rev_id){
  return get_sandbox_path() + "/problem_data/" + problem_id + "." + std::to_string(rev_id)+"/metadata/tests.gen";
}

std::string architecture_utilities::get_problem_validator_exec_path(const std::string& problem_id, int rev_id, const std::string& val_exec_name){
  return get_sandbox_path() + "/problem_data/" + problem_id + "." + std::to_string(rev_id)+"/files/validators/"+val_exec_name;
}

std::string architecture_utilities::get_problem_generator_exec_path(const std::string& problem_id, int rev_id, const std::string& gen_exec_name){
  return get_sandbox_path() + "/problem_data/" + problem_id + "." + std::to_string(rev_id)+"/files/generators/"+gen_exec_name;
}

std::string architecture_utilities::get_problem_source_exec_path(const std::string& problem_id, int rev_id, const std::string& src_exec_name){
  return get_sandbox_path() + "/problem_data/" + problem_id + "." + std::to_string(rev_id)+"/files/sources/"+src_exec_name;
}