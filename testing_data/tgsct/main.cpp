#include "../../headers/Tasks/test_generation_script_compiler_task.h"
#include <bits/stdc++.h>
#include <Utilities/architecture_utilities.h>
using namespace std;

int main(){
    cout<<architecture_utilities::get_problem_data_folder("swapsort",1)<<'\n';
    tgsct t("swapsort", 1);
    result_enum retval=t.execute(0,0);
    cout<<(int)retval<<'\n';
    cout<<t.get_compilation_message(30);
    if(retval!=result_enum::OK) return 0;
    problem_metadata output = t.get_problem_metadata();
    
    cout<<"Time limit: "<<output.time_limit<<"s\n";
    cout<<"Memory limit: "<<output.memory_limit<<"B\n";
    cout<<"Total points: "<<output.total_points<<'\n';
    cout<<"Subtask count: "<<output.group_count<<'\n';
    for(int i=0;i<output.groups.size();i++){
        cout<<"Group "<<i<<": "<<output.groups[i].total_points<<" points, "<<output.groups[i].test_count<<" tests\n";
    }
    cout<<"Test count: "<<output.test_count<<'\n';
    for(int i=0;i<output.tests.size();i++){
        cout<<"TEST "<<i<<'\n';
        cout<<"Groups: ";
        for(auto it : output.tests[i].groups){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Generator args: ";
        for(auto it : output.tests[i].generator_args){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Validator args: ";
        for(auto it : output.tests[i].validator_args){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Main exe: "<<output.tests[i].main_path;
        cout<<'\n';
        cout<<"Checker args: ";
        for(auto it : output.tests[i].checker_args){
            cout<<it<<' ';
        }
        cout<<'\n';
        cout<<"Interactor args: ";
        for(auto it : output.tests[i].interactor_args){
            cout<<it<<' ';
        }
        cout<<"\n\n";
    }
    
}