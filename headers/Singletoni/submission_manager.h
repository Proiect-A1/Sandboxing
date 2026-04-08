#ifndef SUBMISSION_MANAGER_H
#define SUBMISSION_MANAGER_H
#include<unordered_map>
#include <Chestii_cu_data/submission_data.h>
#include <Chestii_cu_data/submission_test.h>
#include <pthread.h>
class submission_manager{ // singleton
    static submission_manager* instance;
    static pthread_mutex_t mtx;
    std::map<std::string, submission_data> submission_table; /// submission_id -> submission_data
    submission_manager() = default;
public:
    static submission_manager& get_instance();
    std::map<std::string, submission_data> get_submission_table(); // creeaza o copie a submission_table
    void insert(std::string submission_id, std::string problem_id, int rev_id, int socket_fd); /// aduaga submisia in map; PE VIITOR TREBUIE BAGATA SI IN problem_manager
    void erase(std::string submission_id); /// sterge o sursa din map
    bool count(std::string submission_id); /// verifica daca exista sursa in map
    bool is_done(std::string submission_id); /// TEMPORAR E ECHIVALENT CU !COUNT, NU DISTINGE DINTRE SURSE INEXISTENTE SI SURSE TERMINATE
    void add_completed_test(std::string submission_id, int test_id, submission_test test_result); /// marcheaza ca s-a terminat de evaluat un test
};

#endif
