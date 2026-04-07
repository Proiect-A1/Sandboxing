
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
  if (chroot(user_dir.c_str()) != 0)
  {
    return false;
  }
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


bool utilities::copy_file(const std::string &from, const std::string &to, mode_t mode)
{
  std::error_code ec;
  std::filesystem::copy_file(from, to, std::filesystem::copy_options::overwrite_existing, ec);
  if (ec)
  {
    return false;
  }

  return chmod(to.c_str(), mode) == 0;
}

void utilities::print_enum(result_enum result){
  switch(result) {
      case result_enum::OK: std::cout << "OK"; break;
      case result_enum::WA: std::cout << "WA"; break;
      case result_enum::PA: std::cout << "PA"; break;
      case result_enum::TLE: std::cout << "TLE"; break;
      case result_enum::MLE: std::cout << "MLE"; break;
      case result_enum::RTE: std::cout << "RTE"; break;
      case result_enum::CPE: std::cout << "CPE"; break;
      case result_enum::FAIL: std::cout << "FAIL"; break;
      case result_enum::SKIP: std::cout << "SKIP"; break;
      case result_enum::ILE: std::cout << "ILE"; break;
      case result_enum::NONE: std::cout << "NONE"; break;
      case result_enum::OTHER: std::cout << "OTHER"; break;
    }
}