#ifndef VALIDATOR_TASK_HPP
#define VALIDATOR_TASK_HPP
#include <Tasks/task.h>

class validator_task : public task{
  int problem_id;
  int rev_id;
  std::vector<std::string> args; // comanda + argumente pt programul de validare
  std::string input_path;
public:
    validator_task(int problem_id, int rev_id, std::vector<std::string> args, std::string input_path) : problem_id(problem_id), rev_id(rev_id), args(args), input_path(input_path) {
      priority = 1; // Higher priority than grading tasks
    }
    result_enum execute(pthread_t thread_id, int user_id) override;
    bool check_permissions() override { return true; };
};

#endif // VALIDATOR_TASK_HPP