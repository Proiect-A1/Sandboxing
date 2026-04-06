#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <unistd.h>
namespace utilities{
  bool change_root_to_user(std::string username); 
  bool change_root_to_sandbox(); 
  bool change_dir_to_user(std::string username); 
  bool change_dir_to_sandbox(); 
}



#endif // UTILITIES_H