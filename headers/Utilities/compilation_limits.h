#ifndef COMPILATION_LIMITS_H
#define COMPILATION_LIMITS_H

#include <unordered_map>
#include <Enums/language_enum.h>

namespace compilation_limits{
  // in miliseconds
  extern const std::unordered_map<language_enum, int> COMPILATION_TIME_LIMIT;
  // in bytes
  extern const std::unordered_map<language_enum, long long> EXECUTABLE_SIZE_LIMIT;
}


#endif // COMPILATION_LIMITS_H