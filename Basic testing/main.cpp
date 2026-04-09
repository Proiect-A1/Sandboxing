#include <Tasks/evaluator_task.h>
#include <Tasks/stdio_grader_task.h>
#include <iostream>
#include <Utilities/utilities.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <vector>
#include <Singletoni/user_queue.h>
#include <Singletoni/task_queue.h>
using namespace std;

int main(){

  problem_manager& pm = problem_manager::get_instance();
  submission_manager& sm = submission_manager::get_instance();

  problem_metadata new_problem;
  new_problem.problem_id = "Problem";
  new_problem.rev_id = 0;
  new_problem.group_count = 10;
  new_problem.test_count = 10;
  new_problem.time_limit = 1.0;
  new_problem.memory_limit = 256 * 1024 * 1024; // 256 MB
  new_problem.total_points = 100.0;
  new_problem.problem_status = problem_status_enum::DONE;
  new_problem.groups = std::vector<group_metadata>(10, {10.0, 1, group_type_enum::GROUP_MIN});
  new_problem.tests = std::vector<test_metadata>(10, {"", "", "", {}});
  for (int i = 0; i < 10; i++){
    new_problem.tests[i].groups.push_back(i);
  }

  pm.add_revision(new_problem);


  sm.insert("ANDREI", language_enum::CPP, "Problem", 0, 1);

  submission_data submission = sm.get_submission("ANDREI");


  evaluator_task eva("ANDREI", "Problem", 0);

  task_queue::get_instance().push(&eva);

  while (!task_queue::get_instance().empty()){
    auto next_task = task_queue::get_instance().pop();

    if (dynamic_cast<evaluator_task*>(next_task)){
      cout << "Evaluator task popped from queue" << endl;
    } else if (dynamic_cast<stdio_grader_task*>(next_task)){
      cout << "Grader task popped from queue" << endl;
    } else {
      cout << "Unknown task type popped from queue" << endl;

    }
    next_task->execute(0, 1);
  }

  cout << "eyooo" << endl;
  return 0;
}
