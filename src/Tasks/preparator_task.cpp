#include <Tasks/preparator_tasks.h>
#include <Singletoni/problem_manager.h>

bool preparator_task::check_if_problem_exists(){

}
void preparator_task::pull_problem(){
    
}
void preparator_task::compile_problem_sources(){

}
void preparator_task::generate_tests(){

}
result_enum preparator_task::execute(){
    if(!check_if_problem_exists())
        pull_problem();
    compile_problem_sources();
}
