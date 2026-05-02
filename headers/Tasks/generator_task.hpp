#ifndef GENERATOR_H
#define GENERATOR_H
#include <string>
#include <problem_metadata.h>
class generator{
    int problem_id;
    int rev_id;
    std::string problem_folder_root_path;
    std::vector<std::string> test_generation_info;
public:
    generator(int problem_id, int rev_id, std::string problem_folder_root_path);
    std::vector<std::string> compile(std::string script_path);
    void execute(std::string script_path);
};
#endif