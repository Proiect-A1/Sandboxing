#ifndef SUBMISSION_GROUP_H 
#define SUBMISSION_GROUP_H
#include <vector>
#include <algorithm>
#include <Chestii_cu_data/group_metadata.h>
#include <Chestii_cu_data/submission_data.h>
#include <Enums/group_type_enum.h>

struct submission_group{
    float total_points; // maximum points
    float point_percentage; // pentru group_min
    float points; // point_total*point_percentage;
    float time_used; // in milliseconds
    long long memory_used; // in bytes
    int tests_completed;
    int test_count;
    group_type_enum group_type;
    submission_data* parent_submission;

    submission_group(){}
    submission_group(const group_metadata& gmd, submission_data* parent_submission){
        total_points=gmd.total_points;
        point_percentage=1;
        points=0;
        time_used=0;
        memory_used=0;
        tests_completed=0;
        test_count=gmd.test_count;
        group_type=gmd.group_type;
        this->parent_submission=parent_submission;
    }
    void add_completed_test(const submission_test& completed_test){
        if(group_type==group_type_enum::GROUP_MIN){
            point_percentage=std::min(point_percentage, completed_test.points);
        }
        else if(group_type==group_type_enum::GROUP_SUM){
            point_percentage=std::min(point_percentage, completed_test.points);
            // PLACEHOLDER, TESTELE NU STIU CATE PUNCTE VALOREAZA
        }
        else if(group_type==group_type_enum::GROUP_MUL){
            point_percentage*=completed_test.points;
        }
        points=total_points*point_percentage;
        tests_completed++;
        time_used=std::max(time_used, completed_test.time_used);
        memory_used=std::max(memory_used, completed_test.memory_used);
    }
    bool is_completed(){
        return tests_completed == test_count;
    }
};



#endif