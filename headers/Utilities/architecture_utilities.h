#ifndef ARCHITECTURE_UTILITIES_H
#define ARCHITECTURE_UTILITIES_H

#include <Enums/language_enum.h>
#include <Utilities/general_utilities.h>

#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <system_error>
namespace architecture_utilities {
  
  bool change_root_to_user(const int& user_id); 
  bool change_root_to_sandbox(); 
  bool change_dir_to_user(const int& user_id); 
  bool change_dir_to_sandbox(); 


  std::string get_weak_user(const int& user_id);
  std::string get_strong_user(const int& user_id);
  std::string get_run_dir(const int& user_id);
  std::string get_submission_dir(std::string submission_id);
  std::string get_sandbox_path();
  std::string get_submission_source_path(std::string submission_id, language_enum language = language_enum::CPP);
  std::string get_submission_exec_path(std::string submission_id, language_enum language = language_enum::CPP);
  std::string get_problem_input_path(std::string submission_id, int rev_id, int test);
  std::string get_problem_correct_output_path(std::string submission_id, int rev_id, int test);
  std::string architecture_utilities::get_problem_data_folder(const std::string& problem_id, int rev_id);
  std::string architecture_utilities::get_problem_script_folder(const std::string& problem_id, int rev_id);
  std::string architecture_utilities::get_problem_validator_exec_path(const std::string& problem_id, int rev_id, const std::string& val_exec_name);
  std::string architecture_utilities::get_problem_generator_exec_path(const std::string& problem_id, int rev_id, const std::string& gen_exec_name);
  std::string architecture_utilities::get_problem_source_exec_path(const std::string& problem_id, int rev_id, const std::string& src_exec_name);
}
#endif // ARCHITECTURE_UTILITIES_H