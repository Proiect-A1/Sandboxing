
#include <Utilities/general_utilities.h>
#include <sys/stat.h>
#include <unistd.h>

std::string general_utilities::left_zero_pad (int number, int width) {
  std::string num_str = std::to_string(number);
  if (num_str.length() >= width) {
    return num_str;
  }
  return std::string(width - num_str.length(), '0') + num_str;
}

bool general_utilities::copy_file(const std::string &from, const std::string &to, mode_t mode)
{
  std::error_code ec;
  std::filesystem::copy_file(from, to, std::filesystem::copy_options::overwrite_existing, ec);
  if (ec)
  {
    return false;
  }

  return chmod(to.c_str(), mode) == 0;
}

std::string general_utilities::enum_to_string(result_enum result){
  switch(result) {
      case result_enum::OK: return "OK";
      case result_enum::WA: return "WA";
      case result_enum::PA: return "PA";
      case result_enum::TLE: return "TLE";
      case result_enum::MLE: return "MLE";
      case result_enum::RTE: return "RTE";
      case result_enum::CPE: return "CPE";
      case result_enum::FAIL: return "FAIL";
      case result_enum::SKIP: return "SKIP";
      case result_enum::ILE: return "ILE";
      case result_enum::NONE: return "NONE";
      case result_enum::OTHER: return "OTHER";
    }
    return "TROLLEZI";
}

bool general_utilities::is_file(const std::string& path){
    struct stat buff;
    if(stat(path.c_str(),&buff)==-1) return false;
    return (buff.st_mode & S_IFMT) == S_IFREG;
}

bool general_utilities::is_folder(const std::string& path){
    struct stat buff;
    if(stat(path.c_str(),&buff)==-1) return false;
    return (buff.st_mode & S_IFMT) == S_IFDIR;
}

bool general_utilities::is_executable(const std::string& path){
    return access(path.c_str(),X_OK)==0; // 0 if successful, -1 otherwise
}