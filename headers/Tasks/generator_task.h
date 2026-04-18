// genereaza testele la o problema
#ifndef GENERATOR_TASK_H
#define GENERATOR_TASK_H
#include <string>
#include <problem_metadata.h>
class generator_task : public task{
    std::string problem_id;
    int rev_id;
public:
    generator_task(const std::string& problem_id, int rev_id);
    result_enum execute() override;
    bool check_permissions() override {return true};
    void print_error(pthread_t thread_id, int user_id, const std::string& message) override;
    void print_log(pthread_t thread_id, int user_id, const std::string& message) override;
};
#endif