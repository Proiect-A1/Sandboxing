#include <Chestii_cu_data/submission_data.h>

submission_data::submission_data(language_enum language, std::string problem_id, int rev_id, int socket_fd){
    problem_metadata pmd=problem_manager::get_instance().get_metadata(problem_id,rev_id);
    points = 0;
    time_used = 0;
    memory_used = 0;
    tests_completed=0;
    test_count=pmd.test_count;
    time_limit=pmd.time_limit;
    memory_limit=pmd.time_limit;
    this->problem_id=problem_id;
    this->rev_id=rev_id;
    this->socket_fd=socket_fd; /// stdout by default
    this->language=language;
    for(const group_metadata& gmd : pmd.groups){
        groups.push_back(submission_group(gmd, this));
    }
    for(const test_metadata& tmd : pmd.tests){
        tests.push_back(submission_test(tmd, this));
    }
}

void submission_data::add_completed_test(int test_id, result_enum result, float points, float time_used, long long memory_used){
    if(this->result==result_enum::OK && result!=result_enum::OK)
        this->result=result;
    tests[test_id].result=result;
    tests[test_id].points=points;
    tests[test_id].time_used=time_used;
    tests[test_id].memory_used=memory_used;
    ++tests_completed;
    for(int group_id : tests[test_id].groups){
        groups[group_id].add_completed_test(result, points, memory_used, time_used);
        if(groups[group_id].is_completed()){
            add_completed_subtask(group_id, groups[group_id]);
        }
    }
    send_completed_test_packet(test_id, tests[test_id]);
    if(tests_completed==test_count){
        send_completed_submission_packet();
    }
        
}
void submission_data::add_completed_subtask(int group_id, const submission_group& completed_group){
    points+=completed_group.points;
    time_used=std::max(time_used, completed_group.time_used);
    memory_used=std::max(memory_used, completed_group.memory_used);
    send_completed_subtask_packet(group_id, completed_group);
}

void submission_data::set_verdict(result_enum result, float points, float time_used, long long memory_used){
    this->result=result;
    this->points=points;
    this->time_used=time_used;
    this->memory_used=memory_used;
    send_completed_submission_packet();
}
void submission_data::send_completed_test_packet(int test_id, const submission_test& completed_test){
    // de trimis packet cu test terminat
    std::cout << "Problem " << problem_id << " rev " << rev_id 
              << " completed test " << test_id <<" with " << completed_test.points 
              << " points, time used: " << completed_test.time_used 
              << " ms, memory used: " << completed_test.memory_used << " B\n";
}
void submission_data::send_completed_subtask_packet(int group_id, const submission_group& completed_group){
    // de trimis packet cu subtask terminat
    std::cout << "Problem " << problem_id << " rev " << rev_id 
              << " completed group " << group_id <<" with " << completed_group.points 
              << " points, time used: " << completed_group.time_used 
              << " ms, memory used: " << completed_group.memory_used << " B\n";
}
void submission_data::send_completed_submission_packet(){
    // de trimis packet cu sursa terminata
    std::cout << "Problem " << problem_id << " rev " << rev_id << " completed with " << points << " points, time used: " << time_used << " ms, memory used: " << memory_used << " B\n";
}