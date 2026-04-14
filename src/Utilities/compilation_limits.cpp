#include <Utilities/compilation_limits.h>

namespace compilation_limits{
  const std::unordered_map<language_enum, int> COMPILATION_TIME_LIMIT = {
      {language_enum::CPP, 10000} // 10 seconds
  };

  const std::unordered_map<language_enum, long long> EXECUTABLE_SIZE_LIMIT = {
      {language_enum::CPP, 512 * 1024 * 1024} // 512MB
  };
}