#ifndef PROBLEM_METADATA_H
#define PROBLEM_METADATA_H

#include <string>
#include <vector>
#include <Enums/problem_status_enum.h>
#include <Chestii_cu_data/group_metadata.h>
#include <Chestii_cu_data/test_metadata.h>

struct problem_metadata {
  problem_status_enum problem_status;
  std::string problem_id;
  std::string founding_submission_id;
  int rev_id;

  int group_count;
  int test_count;
  float time_limit;
  float total_points;
  long long memory_limit;
  std::vector<group_metadata> groups;
  std::vector<test_metadata> tests;
};

#endif // PROBLEM_METADATA_H