#ifndef SUBMISSION_GROUP_H 
#define SUBMISSION_GROUP_H
#include <vector>

struct submission_data;
struct submission_group{
  float point; // percentage of the total points for the group
  float time_used; // in milliseconds
  long memory_used; // in bytes
  int tests_completed;
  
  submission_data* parent_submission;
};



#endif