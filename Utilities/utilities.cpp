
#include <Utilities/utilities.h>

bool utilities::change_root_to_user(std::string username){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path) + "/runs/" + username;
  chroot(user_dir.c_str());
  return true;

}

bool utilities::change_root_to_sandbox(){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path);
  chroot(user_dir.c_str());
  return true;
}
bool utilities::change_dir_to_user(std::string username){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path) + "/runs/" + username;
  chdir(user_dir.c_str());
  return true;

}

bool utilities::change_dir_to_sandbox(){
  const char *sandbox_path = getenv("SANDBOX_PATH");
  if (sandbox_path == nullptr || sandbox_path[0] == '\0')
  {
    return false;
  }
  const std::string user_dir = std::string(sandbox_path);
  chdir(user_dir.c_str());
  return true;
}