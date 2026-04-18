// da pull la o problema si genereaza teste daca e nevoie
#ifndef PREPARATOR_TASK_H
#define PREPARATOR_TASK_H

#include <string>

class preparator_task : public task{
    std::string problem_id;
    int rev_id;
public:
    preparator(const std::string& problem_id, int rev_id);
private:
    bool check_if_problem_exists();
    void pull_problem();
    void compile_problem_sources();
    void generate_tests();
public:
    result_enum execute() override;
    bool check_permissions() override {return true};
    void print_error(pthread_t thread_id, int user_id, const std::string& message) override;
    void print_log(pthread_t thread_id, int user_id, const std::string& message) override;
};
#endif