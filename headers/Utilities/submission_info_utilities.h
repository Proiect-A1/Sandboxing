#ifndef SUBMISSION_INFO_UTILITIES_H
#define SUBMISSION_INFO_UTILITIES_H

#include <string>
#include <Enums/language_enum.h>
namespace submission_info_utilities {
std::string left_zero_pad (int number, int width);
std::string get_run_dir(std::string username);
std::string get_submission_dir(std::string submission_id);
std::string get_submission_source_path(std::string submission_id, language_enum language = language_enum::CPP);
std::string get_submission_exec_path(std::string submission_id, language_enum language = language_enum::CPP);
std::string get_problem_input_path(std::string submission_id, int rev_id, int test);
std::string get_problem_correct_output_path(std::string submission_id, int rev_id, int test);
}
#endif // SUBMISSION_INFO_UTILITIES_H