// genereaza un test :D
#ifndef SINGLE_TEST_GENERATOR_TASK_H
#define SINGLE_TEST_GENERATOR_TASK_H
#include <string>
#include <problem_metadata.h>
class single_test_generator_task : public task {
    std::string problem_id;
    int rev_id;
    int test_id;
    std::string main_exec_path;
    std::vector<std::string> validator_args; // primul element e path-ul pana la validator
public:
    single_test_generator_task(const std::string& problem_id, int rev_id, int test_id, const std::vector<std::string>& validator_args);
    result_enum execute() override;
    bool check_permissions() override {return true};
    void print_error(pthread_t thread_id, int user_id, const std::string& message) override;
    void print_log(pthread_t thread_id, int user_id, const std::string& message) override;
};

typedef single_test_generator_task stgt;
#endif