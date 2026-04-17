#ifndef PREPARATOR_H
#define PREPARATOR_H

#include <string>
class preparator{
    int problem_id;
    int rev_id;
    std::string problem_folder_root_path;
public:
    preparator(int problem_id, int rev_id, std::string problem_folder_root_path);
private:
    bool check_if_problem_exists();
    void pull_problem();
    void generate_tests();
    void compile_problem_sources();
public:
    void execute();
};
#endif