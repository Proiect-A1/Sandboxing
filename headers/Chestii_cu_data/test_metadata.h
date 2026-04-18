#ifndef TEST_METADATA_H
#define TEST_METADATA_H
#include <vector>
#include <string>
struct test_metadata{
    std::vector<std::string> interactor_args; /// primul argument din vector e path-ul
    std::vector<std::string> generator_args;
    std::vector<std::string> validator_args;
    std::vector<int> groups;
};
#endif