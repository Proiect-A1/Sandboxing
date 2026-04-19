#ifndef CHECKER_TASK_H
#define CHECKER_TASK_H

#include <string>
#include <Enums/result_enum.h>
#include <Tasks/task.h>
#include <Utilities/architecture_utilities.h>
class checker_task : public task{
  std::string input;
  std::string output;
  std::string correct_output;
  std::string contestant_source;
  float point_percentage = 0;
  std::string message = "";
  public:
  checker_task(std::string input, std::string output, std::string correct_output, std::string contestant_source)
      : input(input), output(output), correct_output(correct_output), contestant_source(contestant_source), point_percentage(point_percentage) {}
  bool check_permissions() override {
    return true;
  }

  result_enum execute(pthread_t thread_id, int user_id) override;
  float get_point_percentage() const {
    return point_percentage;
  }
  std::string get_message() const {
    return message;
  }
};



#endif // CHECKER_TASK_H