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
  
  bool change_root_to_user(std::string username); 
  bool change_root_to_sandbox(); 
  bool change_dir_to_user(std::string username); 
  bool change_dir_to_sandbox(); 

  std::string get_run_dir(std::string username);
  std::string get_submission_dir(std::string submission_id);
  std::string get_submission_source_path(std::string submission_id, language_enum language = language_enum::CPP);
  std::string get_submission_exec_path(std::string submission_id, language_enum language = language_enum::CPP);
  std::string get_problem_input_path(std::string submission_id, int rev_id, int test);
  std::string get_problem_correct_output_path(std::string submission_id, int rev_id, int test);
}
#endif // ARCHITECTURE_UTILITIES_H