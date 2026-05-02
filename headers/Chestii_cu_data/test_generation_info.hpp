#ifndef TEST_GENERATION_INFO_HPP
#define TEST_GENERATION_INFO_HPP
#include <vector>
#include <string>
struct test_generation_info{
  bool is_raw_file; // 1 pentru test pregenerat, 0 pentru generator
  std::vector<std::string> args; // la test pregenerat o sa fie numele fisierului care trebuie copiat; la generator o sa fie path-ul executabilului si parametrii din command line
  std::vector<std::string> validator_args; // path-ul executabilului + parametrii din command line
};

#endif