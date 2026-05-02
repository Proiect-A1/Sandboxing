#ifndef GROUP_METADATA_H
#define GROUP_METADATA_H

#include <Enums/group_type_enum.h>

struct group_metadata {
  float total_points;
  int test_count;
  group_type_enum group_type;
  group_metadata(){}
  group_metadata(float total_points, int test_count, group_type_enum group_type){
    this->total_points=total_points;
    this->test_count=test_count;
    this->group_type=group_type;
  }
};

#endif // GROUP_METADATA_H