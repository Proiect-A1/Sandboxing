#ifndef SUBMISSION_DATA_H
#define SUBMISSION_DATA_H

#include <vector>
#include <Chestii_cu_data/submission_test.h>
#include <Chestii_cu_data/submission_group.h>
#include <Chestii_cu_data/problem_metadata.h>
#include <Singletoni/problem_manager.h>
#include <Enums/language_enum.h>
struct submission_data{
    language_enum language;

    int tests_completed;
    int test_count; 
    
    float total_points; // punctaj maxim (de regula 100)
    float time_limit; // in milliseconds
    long long memory_limit; // in bytes
    
    float points; // punctaj obtinut
    float time_used;
    long long memory_used;

    std::string problem_id;
    int rev_id;
    int socket_fd; // unde transmite raspunsurile

    std::vector<submission_test> tests;
    std::vector<submission_group> groups;
    submission_data(){}
    submission_data(std::string problem_id, int rev_id, int socket_fd=1);
    void add_completed_test(int test_id, const submission_test& completed_test);
    void add_completed_subtask(const submission_group& completed_group);
};

#endif