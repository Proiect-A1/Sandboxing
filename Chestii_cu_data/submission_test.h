#ifndef SUBMISSION_TEST_H
#define SUBMISSION_TEST_H

#include <result_enum.h>
#include <string>

struct submission_data;

// internal structure for passing information regarding a test
struct submission_test{
  float points; // percentage of the total points for the test
  std::string message; // message to be sent to the user after the test is completed
  float time_used; // in milliseconds
  long memory_used; // in bytes
  result_enum result; 
  const submission_data *parent_submission_id;
};



#endif