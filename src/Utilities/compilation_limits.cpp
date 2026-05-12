#include <Utilities/compilation_limits.h>

namespace compilation_limits{
  const std::unordered_map<language_enum, int> COMPILATION_TIME_LIMIT = {
      {language_enum::CPP, 10000}, // 10 seconds
      {language_enum::RUST, 10000}, // 10 seconds
      {language_enum::C, 10000},
      {language_enum::CSHARP, 30000}, // AOT-ul are nevoie de mai mult timp de compilare aparent
      {language_enum::GO, 10000},
      {language_enum::PYTHON, 10000}
  };

  const std::unordered_map<language_enum, long long> EXECUTABLE_SIZE_LIMIT = {
      {language_enum::CPP, 512 * 1024 * 1024}, // 512MB
      {language_enum::RUST, 512 * 1024 * 1024}, // 512MB
      {language_enum::C, 512 * 1024 * 1024},
      {language_enum::CSHARP, 512 * 1024 * 1024},
      {language_enum::GO, 512 * 1024 * 1024},
      {language_enum::PYTHON, 512 * 1024 * 1024}
  };
}