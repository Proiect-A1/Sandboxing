#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <system_error>
namespace utilities{
  bool change_root_to_user(std::string username); 
  bool change_root_to_sandbox(); 
  bool change_dir_to_user(std::string username); 
  bool change_dir_to_sandbox(); 
  bool copy_file(std::string from, std::string to, mode_t mode);
}



#endif // UTILITIES_H