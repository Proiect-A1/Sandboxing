#ifndef RESULT_ENUM_H
#define RESULT_ENUM_H

enum class result_enum{
  OK, // full points
  WA, // wrong answer
  PE, // presentation error
  SUPER, // better answer then model solution
  PA, // partial answer
  FAIL, // internal error
  SKIP, // skipped test (e.g. due to subtask dependency)
  NONE, // not available
  CPE, // compile error
  TLE, // time limit exceeded
  MLE, // memory limit exceeded
  RTE, // runtime error
  ILE, // idleness limit exceeded
  OTHER // trollezi?
};


#endif