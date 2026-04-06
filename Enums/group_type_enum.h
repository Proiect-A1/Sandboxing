#ifndef GROUP_TYPE_ENUM_H
#define GROUP_TYPE_ENUM_H

enum class group_type_enum {
  GROUPMIN, // points awarded is the minimum of the points obtained on the tests in the group
  GROUPSUM, // points awarded is the sum of the points obtained on the tests in
  GROUPMUL, // points awarded is the product of the points obtained on the tests in the group
};

#endif // GROUP_TYPE_ENUM_H