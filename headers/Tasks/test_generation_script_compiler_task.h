#ifndef TEST_GENERATION_SCRIPT_COMPILER_TASK_H
#define TEST_GENERATION_SCRIPT_COMPILER_TASK_H
#include <string>
#include <vector>
#include <Chestii_cu_data/problem_metadata.h>
#include <Tasks/task.h>
class test_generation_script_compiler_task : public task{
    std::string problem_id;
    int rev_id;
    std::vector<std::string> compilation_message_list; 
    problem_metadata output;
    /// sunt foarte nefericit de arhitectura asta
    /// compilation_message-ul ar trebui sa fie return value-ul de la execute
    /// :D
public:
    test_generation_script_compiler_task(const std::string& problem_id, int rev_id); 
    /// probabil trebuie puse in constructor limitele de timp+memorie pt validator/generator
    void add_fatal_error(const std::string& msg);
    void add_error(int line, const std::string& msg);
    void add_warning(int line, const std::string& msg);
    void add_note(int line, const std::string& msg);
    std::string get_compilation_message(uint32_t max_lines);
    problem_metadata get_problem_metadata();
    result_enum execute(pthread_t thread_id, int user_id) override; /// returneaza OK/result_enum-ul de la oficiala/FAIL
    bool check_permissions() override;
    void print_error(pthread_t thread_id, int user_id, const std::string& message) override;
    void print_log(pthread_t thread_id, int user_id, const std::string& message) override;
};

typedef test_generation_script_compiler_task tgsct;
#endif