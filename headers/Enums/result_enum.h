#ifndef RESULT_ENUM_H
#define RESULT_ENUM_H

enum class result_enum{
  OK, // full points
  WA, // wrong answer
  PA, // partial answer
  TLE, // time limit exceeded
  MLE, // memory limit exceeded
  RTE, // runtime error
  CPE, // compile error
  FAIL, // internal error
  SKIP, // skipped test (e.g. due to subtask dependency)
  ILE, //idleness limit exceeded
  NONE, // not available
  OTHER // trollezi?
};


#endif