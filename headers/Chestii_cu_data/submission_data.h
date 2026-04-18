#ifndef SUBMISSION_DATA_H
#define SUBMISSION_DATA_H

#include <vector>
#include <Chestii_cu_data/submission_test.h>
#include <Chestii_cu_data/submission_group.h>
#include <Chestii_cu_data/problem_metadata.h>
#include <Singletoni/problem_manager.h>
#include <Enums/language_enum.h>
#include <Enums/result_enum.h>
#include <iostream>
struct submission_data{
    language_enum language;

    int tests_completed;
    int test_count; 
    
    float total_points; // punctaj maxim (de regula 100)
    float time_limit; // in milliseconds
    long long memory_limit; // in bytes
    
    result_enum result;
    float points; // punctaj obtinut
    float time_used;
    long long memory_used;

    std::string problem_id;
    int rev_id;
    int socket_fd; // unde transmite raspunsurile

    std::vector<submission_test> tests;
    std::vector<submission_group> groups;
    submission_data(){}
    submission_data(language_enum language, std::string problem_id, int rev_id, int socket_fd=1);
    void add_completed_test(int test_id, result_enum result, float points, float time_used, long long memory_used);
    void add_completed_subtask(int group_id, const submission_group& completed_group);
    void set_verdict(result_enum result, float points, float time_used, long long memory_used);
    // trimite un packet spre db pentru fiecare test cu verdictul SKIP, 0 timp si 0 memorie
    // dupa care trimite un packet spre db pentru fiecare subtask cu verdictul SKIP, 0 timp si 0 memorie
    // dupa care apeleaza mark_completed()
    // set_verdict ar trebui folosit doar pentru CPE (desi probabil merge si ca sa dai skip la o sursa stil codeforces)
    void send_completed_test_packet(int test_id, const submission_test& test);
    void send_completed_subtask_packet(int group_id, const submission_group& group);
    void send_completed_submission_packet();
    // trimite packet-ul spre db care spune ca s-a terminat de evaluat sursa
};

#endif