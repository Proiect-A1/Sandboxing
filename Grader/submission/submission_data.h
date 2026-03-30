#ifndef SUBMISSION_DATA_H
#define SUBMISSION_DATA_H

#include <vector>
#include <submission_test.h>
#include <submission_group.h>
struct submission_data{
  int tests_completed;
  int test_count; 
  float time_limit; // in milliseconds
  long memory_limit; // in bytes
  long problem_id;
  int rev_id;
  int socketfd; // unde transmite raspunsurile


  // std::vector<submission_test> tests;
  // std::vector<submission_group> groups;
  submission_data(int problem_id, int rev_id);
};

#endif