#ifndef PROBLEM_METADATA_H
#define PROBLEM_METADATA_H

#include <problem_status_enum.h>

struct problem_metadata{
  problem_status_enum status;
  long problem_id;
  int rev_id;
  int test_count;
  float total_points;
  long memory_limit; // in bytes
  float time_limit; // in milliseconds

};

#endif