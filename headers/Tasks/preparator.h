#ifndef PREPARATOR_H
#define PREPARATOR_H

#include <string>
#include <Tasks/task.h>

class preparator : public task{
    std::string problem_id;
    int rev_id;
public:
    preparator(std::string problem_id, int rev_id);
private:
    bool check_if_problem_exists();
    void pull_problem();
    void generate_tests();
    void compile_problem_sources();
public:
    result_enum execute(pthread_t thread_id, int user_id) override;
    bool check_permissions(int user_id) override;
};
#endif