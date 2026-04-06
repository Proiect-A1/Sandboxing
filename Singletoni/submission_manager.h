#ifndef SUBMISSION_MANAGER_H
#define SUBMISSION_MANAGER_H
#include<unordered_map>
#include<Chestii_cu_data/submission_data.h>
#include<Chestii_cu_data/submission_test.h>
class submission_manager{ // singleton
    static submission_manager* instance;
    static pthread_mutex_t lock;
    std::unordered_map<long, submission_data> submission_table; /// submission_id -> submission_data
    submission_manager() = default;
public:
    static submission_manager& get_instance();
    void insert(long submission_id, int problem_id, int rev_id); /// aduaga submisia in map; PE VIITOR TREBUIE BAGATA SI IN problem_manager
    void erase(long submission_id); /// sterge o sursa din map
    bool count(long submission_id); /// verifica daca exista sursa in map
    bool is_done(long submission_id); /// TEMPORAR E ECHIVALENT CU !COUNT, NU DISTINGE DINTRE SURSE INEXISTENTE SI SURSE TERMINATE
    void set_test_result(long submission_id, int test_id, submission_test test_result); /// marcheaza ca s-a terminat de evaluat un test
};

#endif
