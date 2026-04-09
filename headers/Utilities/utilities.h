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
  std::string enum_to_string(result_enum result);
  template <typename T>
  struct pointer_compare {
    bool operator()(const T* a, const T* b) const {
      return *a < *b; // Assuming T has operator< defined
    }
  };
}



#endif // UTILITIES_H