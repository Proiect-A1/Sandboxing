#ifndef GROUP_TYPE_ENUM_H
#define GROUP_TYPE_ENUM_H

enum class group_type_enum {
  GROUP_MIN, // points awarded is the minimum of the points obtained on the tests in the group
  GROUP_SUM, // points awarded is the sum of the points obtained on the tests in
  GROUP_MUL, // points awarded is the product of the points obtained on the tests in the group
};

#endif // GROUP_TYPE_ENUM_H