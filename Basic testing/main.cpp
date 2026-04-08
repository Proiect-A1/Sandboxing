#include <Tasks/evaluator_task.h>
#include <Tasks/stdio_grader_task.h>
#include <iostream>
#include <Utilities/utilities.h>
using namespace std;

int main(){

  stdio_grader_task grader("ANDREI", "Problem", 0, 0);
  cout << utilities::enum_to_string(grader.execute(0, 1)) << endl;

  // cout << "eyooo" << endl;
  return 0;
}