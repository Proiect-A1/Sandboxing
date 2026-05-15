#ifndef PROBLEM_STATUS_ENUM_H
#define PROBLEM_STATUS_ENUM_H

enum class problem_status_enum{
  NOT_EXISTS, // not loaded locally
  DOWNLOADING, // downloading from the DB
  COMPILING, // compiling sources
  DOWNLOADED, // downloaded but not processed
  GENERATING, // generating test data
  DONE, // problem is ready for use 
  FAILED, // problem failed to be generated
};

#endif