#ifndef PROBLEM_METADATA_H
#define PROBLEM_METADATA_H

#include <string>
#include <Enums/problem_status_enum.h>
#include <Chestii_cu_data/group_metadata.h>
#include <Chestii_cu_data/test_metadata.h>

struct problem_metadata {
  problem_status_enum problem_status;
  long founding_submission_id;
  float time_limit;
  long memory_limit;
  long problem_id;
  int rev_id;
  int test_count;
  float total_points;
  int group_count;
  std::vector<group_metadata> groups;
  std::vector<test_metadata> tests;
};

#endif // PROBLEM_METADATA_H