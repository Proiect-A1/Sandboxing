#ifndef TEST_METADATA_H
#define TEST_METADATA_H
#include <vector>
#include <string>
struct test_metadata{
    std::string main_path;
    std::vector<std::string> generator_args;
    std::vector<std::string> validator_args;
    std::vector<std::string> interactor_args; /// primul argument din vector e path-ul
    std::vector<std::string> checker_args;
    std::vector<int> groups;
    test_metadata(){}
    test_metadata(const std::string& main_path, 
                  const std::vector<std::string>& generator_args,
                  const std::vector<std::string>& validator_args,
                  const std::vector<std::string>& interactor_args,
                  const std::vector<std::string>& checker_args,
                  const std::vector<int>& groups){
        this->main_path=main_path;
        this->generator_args=generator_args;
        this->validator_args=validator_args;
        this->interactor_args=interactor_args;
        this->checker_args=checker_args;
        this->groups=groups;
    }
};
#endif