#ifndef TEST_METADATA_H
#define TEST_METADATA_H
#include <vector>
struct test_metadata{
    std::string interactor_exec_path;
    std::string checker_exec_path;
    std::string validator_exec_path;
    std::vector<int> groups;
};
#endif