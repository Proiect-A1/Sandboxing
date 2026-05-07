#ifndef GENERAL_UTILITIES_H
#define GENERAL_UTILITIES_H

#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <system_error>
#include <iostream>
#include <Enums/result_enum.h>
#include <Enums/language_enum.h>
namespace general_utilities{
  std::string left_zero_pad (int number, int width);
  bool copy_file(const std::string& from, const std::string& to, mode_t mode);
  bool is_folder(const std::string& path);
  bool is_file(const std::string& path);
  bool is_executable(const std::string& path);
  std::string enum_to_string(result_enum result);
  language_enum string_to_language(const std::string& lang);
  template <typename T>
  struct pointer_compare {
    bool operator()(const T* a, const T* b) const {
      return *a < *b; // Assuming T has operator< defined
    }
  };
  std::string syscall_to_string(std::string syscall);
};



#endif // GENERAL_UTILITIES_H