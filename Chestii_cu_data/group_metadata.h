#ifndef GROUP_METADATA_H
#define GROUP_METADATA_H

#include <Enums/group_type_enum.h>

struct group_metadata {
  float total_points;
  int test_count;
  group_type_enum group_type;
};

#endif // GROUP_METADATA_H