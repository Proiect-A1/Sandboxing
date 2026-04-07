#include <Chestii_cu_data/submission_data.h>
submission_data::submission_data(std::string problem_id, int rev_id, int socket_fd=1){
    problem_metadata pmd=problem_manager::get_instance().get_metadata(problem_id,rev_id);
    tests_completed=0;
    test_count=pmd.test_count;
    time_limit=pmd.time_limit;
    memory_limit=pmd.time_limit;
    this->problem_id=problem_id;
    this->rev_id=rev_id;
    this->socket_fd=socket_fd; /// stdout by default
    for(const group_metadata& gmd : pmd.groups){
        groups.push_back(submission_group(gmd, this));
    }
    for(const test_metadata& tmd : pmd.tests){
        tests.push_back(submission_test(tmd, this));
    }
}

void submission_data::add_completed_test(int test_id, const submission_test& completed_test){
    tests[test_id]=completed_test; /// aici trebuie vazut daca sunt copiate bine subtask-urile
    ++tests_completed;
    for(int group_id : tests[test_id].groups){
        groups[group_id].add_completed_test(completed_test);
        if(groups[group_id].is_completed()){
            add_completed_subtask(groups[group_id]);
        }
    }
    // de trimis packet cu testul terminat
    if(tests_completed==test_count){
        // de trimis packet cu submisia terminata
    }
        
}
void submission_data::add_completed_subtask(const submission_group& completed_group){
    points+=completed_group.points;
    time_used=std::max(time_used, completed_group.time_used);
    memory_used=std::max(memory_used, completed_group.memory_used);
    // de trimis packet cu grup terminat
}