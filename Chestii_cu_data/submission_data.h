#ifndef SUBMISSION_DATA_H
#define SUBMISSION_DATA_H

#include <vector>
#include <Enums/language_enum.h>
#include <Chestii_cu_data/submission_test.h>
#include <Chestii_cu_data/submission_group.h>
struct submission_data{
  language_enum language;
  int tests_completed;
  int test_count; 
  float time_limit; // in milliseconds
  long memory_limit; // in bytes
  std::string submission_id;
  std::string problem_id;
  int rev_id;
  int socketfd; // unde transmite raspunsurile


  // std::vector<submission_test> tests;
  // std::vector<submission_group> groups;
  // submission_data(int problem_id, int rev_id){}
};

#endif