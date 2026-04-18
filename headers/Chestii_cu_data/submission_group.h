#ifndef SUBMISSION_GROUP_H 
#define SUBMISSION_GROUP_H
#include <vector>
#include <algorithm>
#include <Chestii_cu_data/group_metadata.h>
#include <Chestii_cu_data/submission_data.h>
#include <Enums/group_type_enum.h>

struct submission_group{
    float total_points=0; // maximum points
    float point_percentage=0; // pentru group_min
    float points=0; // point_total*point_percentage;
    result_enum result=result_enum::OK; // poate au nevoie in frontend idk
    float time_used=0; // in milliseconds
    long long memory_used=0; // in bytes
    int tests_completed=0;
    int test_count=0;
    group_type_enum group_type=group_type_enum::GROUP_MIN;
    submission_data* parent_submission=NULL;

    submission_group(){}
    submission_group(const group_metadata& gmd, submission_data* parent_submission){
        total_points=gmd.total_points;
        point_percentage=1;
        points=0;
        result=result_enum::OK;
        time_used=0;
        memory_used=0;
        tests_completed=0;
        test_count=gmd.test_count;
        group_type=gmd.group_type;
        this->parent_submission=parent_submission;
    }
    void add_completed_test(result_enum result, float points, float time_used, long long memory_used){
        if(this->result==result_enum::OK && result!=result_enum::OK)
            this->result=result;
        if(group_type==group_type_enum::GROUP_MIN){
            point_percentage=std::min(point_percentage, points);
        }
        else if(group_type==group_type_enum::GROUP_SUM){
            point_percentage=std::min(point_percentage, points);
            // GROUP SUM e o minciuna capitalista si nu exista de fapt
        }
        else if(group_type==group_type_enum::GROUP_MUL){
            point_percentage*=points;
        }
        this->points=total_points*point_percentage;
        tests_completed++;
        this->time_used=std::max(this->time_used, time_used);
        this->memory_used=std::max(this->memory_used, memory_used);
    }
    bool is_completed(){
        return tests_completed == test_count;
    }
};



#endif