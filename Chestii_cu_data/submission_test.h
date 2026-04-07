#ifndef SUBMISSION_TEST_H
#define SUBMISSION_TEST_H

#include <Enums/result_enum.h>
#include <Chestii_cu_data/test_metadata.h>
#include <string>

struct submission_data;

// internal structure for passing information regarding a test
struct submission_test{
    float points; // percentage of the total points for the test (between 0 and 1)
    std::string message; // message to be sent to the user after the test is completed
    float time_used; // in milliseconds
    long memory_used; // in bytes
    result_enum result; 
    std::vector<int> groups;
    submission_data *parent_submission;
    submission_test(){}
    submission_test(const test_metadata& tmd, submission_data* parent_submission){
        points=0;
        time_used=0;
        memory_used=0;
        result=result_enum::NONE;
        groups=tmd.groups;
        this->parent_submission=parent_submission;
    }
};



#endif