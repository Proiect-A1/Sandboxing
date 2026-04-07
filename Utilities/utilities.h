#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <system_error>
#include <iostream>
#include <Enums/result_enum.h>
namespace utilities{
  bool change_root_to_user(std::string username); 
  bool change_root_to_sandbox(); 
  bool change_dir_to_user(std::string username); 
  bool change_dir_to_sandbox(); 
  bool copy_file(const std::string& from, const std::string& to, mode_t mode);
  void print_enum(result_enum result);
}



#endif // UTILITIES_H