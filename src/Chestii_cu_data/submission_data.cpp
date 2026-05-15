#include <Chestii_cu_data/submission_data.h>
#include <Singletoni/submission_manager.h>
#include <Singletoni/logger.h>
#include <Server/IO.hpp>

submission_data::submission_data(std::string submission_id , language_enum language, std::string problem_id, int rev_id, std::string download_link, int socket_fd){
    problem_metadata pmd=problem_manager::get_instance().get_metadata(problem_id,rev_id);
    points = 0;
    time_used = 0;
    memory_used = 0;
    tests_completed=0;
    test_count=pmd.test_count;
    time_limit=pmd.time_limit;
    memory_limit=pmd.memory_limit;
    this->problem_id=problem_id;
    this->rev_id=rev_id;
    this->socket_fd=socket_fd; /// stdout by default
    this -> download_link = download_link;
    this->language=language;
    this -> submission_id = submission_id;
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
        LOG_DEBUG("fur curent");
        // de trimis packet cu submisia terminata
        send_completed_submission_packet();
    }
        
}
void submission_data::add_completed_test(int test_id, submission_test res){
    add_completed_test(test_id, res.result, res.points, res.time_used, res.memory_used);
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
    //LOG_DEBUG(std::string("Submission ") + submission_id + " rev " + std::to_string(rev_id) +
    //            " completed test " + std::to_string(test_id) + " with " + std::to_string(points) +
    //            " points, time used: " + std::to_string(time_used) + " ms, memory used: " +
    //            std::to_string(memory_used) + " B\n" + "Completed " + std::to_string(tests_completed) + "/" + std::to_string(test_count) + " tests");
    //TREBUIE MODIFICAT CU TOTI PARAMETRII BUNI
    //IO::done_test_request(submission_id , test_id , (int)completed_test.result, completed_test.message, points, memory_used , time_used , socket_fd);
}
void submission_data::send_completed_subtask_packet(int group_id, const submission_group& completed_group){
    // de trimis packet cu subtask terminat
    //IO::done_subtask_request(this->submission_id, group_id, completed_group.points, completed_group.total_points, completed_group.point_percentage, completed_group.memory_used, completed_group.time_used, this->socket_fd);
    // TODO: de pus un log_debug corect
    //LOG_DEBUG(std::string("Submission ") + submission_id + ", problem " + problem_id + " rev " + std::to_string(rev_id) +
    //            " completed group " + std::to_string(group_id) + " with " + std::to_string(completed_group.points) +
    //            " points, time used: " + std::to_string(time_used) + " ms, memory used: " +
    //            std::to_string(memory_used) + " B\n" + "Completed " + std::to_string(completed_group.tests_completed) + "/" + std::to_string(completed_group.test_count) + " tests");
    //std::cout << "Problem " << problem_id << " rev " << rev_id 
    //          << " completed group " << group_id <<" with " << completed_group.points 
    //          << " points, time used: " << completed_group.time_used 
    //          << " ms, memory used: " << completed_group.memory_used << " B\n";
}
void submission_data::send_completed_submission_packet(){
    LOG_INFO("fur curent 2");
    submission_manager::get_instance().unsafe_erase(this->submission_id);
    LOG_INFO(std::string("Submission ") + submission_id + " " + problem_id  + " rev " + std::to_string(rev_id) +
                 " completed with " + std::to_string(this->points) + " points, time used: " +
                 std::to_string(this->time_used) + " ms, memory used: " +
                 std::to_string(this->memory_used) + " B");
    // de trimis packet cu sursa terminata
    float total_points=problem_manager::get_instance().get_metadata(problem_id, rev_id).total_points;
    //IO::done_submission_request(this->submission_id , this -> points, total_points, this->points/total_points*100 , this -> memory_used , this -> time_used , this -> socket_fd);
    //std::cout << "Problem " << problem_id << " rev " << rev_id << " completed with " << points << " points, time used: " << time_used << " ms, memory used: " << memory_used << " B\n";
}