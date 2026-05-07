#ifndef TEST_GENERATION_SCRIPT_COMPILER_TASK_H
#define TEST_GENERATION_SCRIPT_COMPILER_TASK_H
#include <string>
#include <vector>
#include <Chestii_cu_data/problem_metadata.h>
#include <Tasks/task.h>
class test_generation_script_compiler_task : public task{
    bool failed = false;
    std::string problem_id;
    int rev_id;
    std::vector<std::string> compilation_message_list; 
    problem_metadata output;
    /// sunt foarte nefericit de arhitectura asta
    /// compilation_message-ul ar trebui sa fie return value-ul de la execute
    /// :D
    void add_fatal_error(const std::string& msg);
    void add_fatal_error(int line, const std::string& msg);
    void add_error(int line, const std::string& msg);
    void add_warning(int line, const std::string& msg);
    void add_note(int line, const std::string& msg);
    bool validate_main_args(int line_no, const std::vector<std::string>& tokens);
    bool validate_validator_args(int line_no, const std::vector<std::string>& tokens);
    bool validate_generator_args(int line_no, const std::vector<std::string>& tokens);
    bool validate_checker_args(int line_no, const std::vector<std::string>& tokens);
    bool validate_interactor_args(int line_no, const std::vector<std::string>& tokens);
    bool validate_in(int line_no, const std::vector<std::string>& tokens);
    bool validate_grp(int line_no, const std::vector<std::string>& tokens);
    bool validate_test(int line_no, const std::vector<std::string>& tokens);
    bool validate_gen_copy(int line_no, const std::vector<std::string>& tokens);
    bool validate_gen_cover(int line_no, const std::vector<std::string>& tokens);
    bool validate_gen(int line_no, const std::vector<std::string>& tokens);
public:
    test_generation_script_compiler_task(const std::string& problem_id, int rev_id); 
    /// probabil trebuie puse in constructor limitele de timp+memorie pt validator/generator
    std::string get_compilation_message(uint32_t max_lines);
    problem_metadata get_problem_metadata();
    result_enum execute(pthread_t thread_id, int user_id) override; /// returneaza OK/FAIL
    bool check_permissions(int user_id) override;
    void print_error(pthread_t thread_id, int user_id,const std::string& message){}
    void print_log(pthread_t thread_id, int user_id,const std::string& message){}
};

typedef test_generation_script_compiler_task tgsct;
#endif