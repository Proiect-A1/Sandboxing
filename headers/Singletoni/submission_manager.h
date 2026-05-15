#ifndef SUBMISSION_MANAGER_H
#define SUBMISSION_MANAGER_H
#include <map>
#include <iostream>
#include <Chestii_cu_data/submission_data.h>
#include <Chestii_cu_data/submission_test.h>
#include <Utilities/general_utilities.h>
#include <Enums/language_enum.h>
#include <pthread.h>
class submission_manager{ // singleton
    static pthread_mutex_t mtx;
    std::map<std::string, submission_data> submission_table; /// submission_id -> submission_data
    submission_manager(){
    };
public:
    static submission_manager& get_instance();
    size_t size();
    std::map<std::string, submission_data> get_submission_table(); // creeaza o copie a submission_table
    submission_data get_submission(std::string submission_id); /// returneaza o copie a submission_data-ului, sau un submission_data gol daca nu exista
    void insert(std::string submission_id, language_enum language, std::string problem_id, int rev_id, std::string download_link , int socket_fd); /// aduaga submisia in map; PE VIITOR TREBUIE BAGATA SI IN problem_manager
    void erase(std::string submission_id); /// sterge o sursa din map
    void unsafe_erase(std::string submission_id); /// sterge o sursa din map FARA LOCK
    bool count(std::string submission_id); /// verifica daca exista sursa in map
    bool is_done(std::string submission_id); /// TEMPORAR E ECHIVALENT CU !COUNT, NU DISTINGE DINTRE SURSE INEXISTENTE SI SURSE TERMINATE
    bool add_completed_test(std::string submission_id, int test_id, submission_test test_result); /// marcheaza ca s-a terminat de evaluat un test
    bool set_verdict(std::string submission_id, result_enum result, float points, float time_used, long long memory_used);
};

#endif
