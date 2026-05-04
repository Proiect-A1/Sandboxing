#include<Tasks/test_generation_script_compiler_task.h>
#include<Utilities/architecture_utilities.h>
#include<Utilities/general_utilities.h>
#include<fstream>
#include<sstream>
#include<map>
#include<cmath>
#include<string>
#include<utility>
#include<set>
#include<vector>
#include<Server/header_helper.hpp>

static float parse_float(const std::string& s){
    size_t ptr=0;
    float f;
    try{
        f=std::stof(s,&ptr);
    }catch(std::out_of_range e){
        return std::nanf("");
    }
    catch(std::invalid_argument e){
        return std::nanf("");
    }
    if(ptr!=s.size()) return std::nanf("");
    return f;
}
tgsct::test_generation_script_compiler_task(const std::string& problem_id, int rev_id){
    this->problem_id = problem_id;
    this->rev_id = rev_id;
    this->failed = false;
}
bool tgsct::check_permissions(){
    return true; /// verific permisiunile in execute in mai multe locuri
}
void tgsct::add_fatal_error(const std::string& msg){
    compilation_message_list.push_back("Fatal error: " + msg);
    this->failed = true;
}
void tgsct::add_fatal_error(int line, const std::string& msg){
    compilation_message_list.push_back("[Line "+std::to_string(line)+"] Fatal error: " + msg);
    this->failed = true;
}
void tgsct::add_error(int line, const std::string& msg){
    compilation_message_list.push_back("[Line "+std::to_string(line)+"] Error: " + msg);
    this->failed = true;
}
void tgsct::add_warning(int line, const std::string& msg){
    compilation_message_list.push_back("[Line "+std::to_string(line)+"] Warning: " + msg);
}
void tgsct::add_note(int line, const std::string& msg){
    compilation_message_list.push_back("[Line "+std::to_string(line)+"] Note: " + msg);
}
std::string tgsct::get_compilation_message(uint32_t max_lines){
    std::string compilation_output;
    for(uint32_t i=0;i<this->compilation_message_list.size() && i<max_lines;i++){
        compilation_output += compilation_message_list[i];
        compilation_output.push_back('\n');
    }
    return compilation_output;
}
problem_metadata tgsct::get_problem_metadata(){
    return this->output;
}
bool tgsct::validate_main_args(int line_no, const std::vector<std::string>& tokens){
    if(tokens.size()!=2){
        add_error(line_no, "Main executable name must not contain spaces; command line arguments are not allowed for main sources");
        add_note(line_no, "Usage example: #MAIN main");
        return false;
    }
    if(tokens[1].find_first_of('.')!=std::string::npos){
        add_error(line_no, "Main executable name must not contain a dot ('.'); do not use file extensions in the executable name");
        add_note(line_no, "Usage example: #MAIN main");
        return false;
    }
    if(!general_utilities::is_file(architecture_utilities::get_problem_source_exec_path(problem_id, rev_id, tokens[1]))){
        add_error(line_no, "Main executable '"+tokens[1]+"' does not exist");
        return false;
    }
    if(!general_utilities::is_executable(architecture_utilities::get_problem_source_exec_path(problem_id, rev_id, tokens[1]))){
        add_fatal_error(line_no, "Main executable '"+tokens[1]+"' cannot be run");
        return false;
    }
    return true;
}
bool tgsct::validate_validator_args(int line_no, const std::vector<std::string>& tokens){
    if(tokens.size()<2){
        add_error(line_no, "Validator name unspecified");
        add_note(line_no, "Usage example: #VAL val [arg1 arg2 ...]");
        return false;
    }
    if(tokens[1].find_first_of('.')!=std::string::npos){
        add_error(line_no, "Validator executable name must not contain a dot ('.'); do not use file extensions in the executable name");
        add_note(line_no, "Usage example: #VAL val [arg1 arg2 ...]");
        return false;
    }
    if(!general_utilities::is_file(architecture_utilities::get_problem_validator_exec_path(problem_id, rev_id, tokens[1]))){
        add_error(line_no, "Validator executable '"+tokens[1]+"' does not exist");
        return false;
    }
    if(!general_utilities::is_executable(architecture_utilities::get_problem_validator_exec_path(problem_id, rev_id, tokens[1]))){
        add_fatal_error(line_no, "Validator executable '"+tokens[1]+"' cannot be run");
        return false;
    }
    return true;
}
bool tgsct::validate_generator_args(int line_no, const std::vector<std::string>& tokens){
    if(tokens.size()<2){
        add_error(line_no, "Generator name unspecified");
        add_note(line_no, "Usage example: #GEN gen [arg1 arg2 ...]");
        return false;
    }
    if(tokens[1].find_first_of('.')!=std::string::npos){
        add_error(line_no, "Generator executable name must not contain a dot ('.'); do not use file extensions in the executable name");
        add_note(line_no, "Usage example: #GEN gen [arg1 arg2 ...]");
        return false;
    }
    if(!general_utilities::is_file(architecture_utilities::get_problem_generator_exec_path(problem_id, rev_id, tokens[1]))){
        add_error(line_no, "Generator executable '"+tokens[1]+"' does not exist");
        return false;
    }
    if(!general_utilities::is_executable(architecture_utilities::get_problem_generator_exec_path(problem_id, rev_id, tokens[1]))){
        add_fatal_error(line_no, "Generator executable '"+tokens[1]+"' cannot be run");
        return false;
    }
    return true;
}
bool tgsct::validate_checker_args(int line_no, const std::vector<std::string>& tokens){
    if(tokens.size()<2){
        add_error(line_no, "Checker name unspecified");
        add_note(line_no, "Usage example: #CHECK checker [arg1 arg2 ...]");
        return false;
    }
    if(tokens[1].find_first_of('.')!=std::string::npos){
        add_error(line_no, "Checker executable name must not contain a dot ('.'); do not use file extensions in the executable name");
        add_note(line_no, "Usage example: #CHECK checker [arg1 arg2 ...]");
        return false;
    }
    if(!general_utilities::is_file(architecture_utilities::get_problem_checker_exec_path(problem_id, rev_id, tokens[1]))){
        add_error(line_no, "Checker executable '"+tokens[1]+"' does not exist");
        return false;
    }
    if(!general_utilities::is_executable(architecture_utilities::get_problem_checker_exec_path(problem_id, rev_id, tokens[1]))){
        add_fatal_error(line_no, "Checker executable '"+tokens[1]+"' cannot be run");
        return false;
    }
    return true;
}
bool tgsct::validate_interactor_args(int line_no, const std::vector<std::string>& tokens){
    if(tokens.size()<2){
        add_error(line_no, "Interactor name unspecified");
        add_note(line_no, "Usage example: #CHECK checker [arg1 arg2 ...]");
        return false;
    }
    if(tokens[1].find_first_of('.')!=std::string::npos){
        add_error(line_no, "Interactor executable name must not contain a dot ('.'); do not use file extensions in the executable name");
        add_note(line_no, "Usage example: #CHECK checker [arg1 arg2 ...]");
        return false;
    }
    if(!general_utilities::is_file(architecture_utilities::get_problem_interactor_exec_path(problem_id, rev_id, tokens[1]))){
        add_error(line_no, "Interactor executable '"+tokens[1]+"' does not exist");
        return false;
    }
    if(!general_utilities::is_executable(architecture_utilities::get_problem_interactor_exec_path(problem_id, rev_id, tokens[1]))){
        add_fatal_error(line_no, "Interactor executable '"+tokens[1]+"' cannot be run");
        return false;
    }
    return true;
}

result_enum tgsct::execute(pthread_t thread_id, int user_id){
    std::string problem_folder=architecture_utilities::get_problem_data_folder(problem_id, rev_id);
    if(!general_utilities::is_folder(problem_folder)){
        add_fatal_error("Problem folder does not exist");
        return result_enum::FAIL;
    }
    std::string metadata_path=architecture_utilities::get_problem_metadata_path(problem_id, rev_id);
    std::string script_path=architecture_utilities::get_problem_script_path(problem_id, rev_id);  
    if(!general_utilities::is_file(metadata_path)){
        add_fatal_error("Metadata file not found");
        return result_enum::FAIL;
    }

    std::ifstream fin(metadata_path.c_str());
    if(!fin.is_open()){
        add_fatal_error("Could not open metadata file");
        return result_enum::FAIL;
    }

    json J;
    fin>>J;
    if(!J.contains("timeLimit")){
        add_fatal_error("Time limit not specified in metadata file");
        return result_enum::FAIL;
    }
    if(!J.contains("memoryLimit")){
        add_fatal_error("Memory limit not specified in metadata file");
        return result_enum::FAIL;
    }
    float tl=J["timeLimit"].get<float>();
    long long ml=J["memoryLimit"].get<long long>();
    if(!general_utilities::is_file(script_path)){
        add_fatal_error("Script file not found");
        return result_enum::FAIL;
    }
    fin.close();
    fin.open(script_path.c_str());
    if(!fin.is_open()){
        add_fatal_error("Could not open script file");
        return result_enum::FAIL;
    }
    
    
    int line_no=0;
    int test_no=0;
    int group_directive_gid=-1;
    std::map<std::string, int> group_ids;
    std::vector<int> group_test_count;
    std::map<std::string, int> group_values;
    std::set<int> current_groups;
    std::string main_path;
    std::vector<std::string> gen_args;
    std::vector<std::string> val_args;
    std::vector<std::string> checker_args;
    std::vector<std::string> interactor_args;
    std::string line;
    output=problem_metadata();
    output.time_limit = tl;
    output.memory_limit = ml;
    output.rev_id=this->rev_id;
    output.problem_id=this->problem_id;
    auto cover_gen = [&](int line_no, const std::vector<std::string>& tokens, std::vector<std::string>& out_args) -> bool {
        if(validate_generator_args(line_no, tokens)){
            out_args.clear();
            for(int i=1;i<tokens.size();i++)
                out_args.push_back(tokens[i]);
            return true;
        }
        return false;
    };
    auto add_test = [&](int line_no, const std::vector<std::string>& tokens) -> bool {
        if(tokens.empty()){
            add_error(line_no, "Empty generator script");
            return false;
        }
        std::vector<std::string> current_gen_args;
        if(tokens[0]=="="){
            if(tokens.size()!=2){
                add_error(line_no, "Invalid copy syntax");
                add_note(line_no, "Usage example: = example.txt");
                return false;
            }
            if(tokens[1].find_first_of("\\/")!=std::string::npos){
                add_error(line_no, "Filename must not contain / or \\");
                add_note(line_no, "Usage example: = example.txt");
                return false;
            }
            if(!general_utilities::is_file(architecture_utilities::get_problem_raw_test_path(problem_id, rev_id, tokens[1]))){
                add_error(line_no, "File '"+tokens[1]+"' not found");
                add_note(line_no, "Usage example: = example.txt");
                return false;
            }
            current_gen_args=tokens;
        }
        else if(tokens[0]=="<"){
            if(tokens.size()<2){
                add_error(line_no, "Invalid generator cover syntax");
                add_note(line_no, "Correct syntax: < gen arg1 arg2 ...");
                return false;
            }
            cover_gen(line_no, tokens, current_gen_args);
        }
        else{
            if(gen_args.empty()){
                add_error(line_no, "No generator specified for this test");
                return false;
            }
            current_gen_args=gen_args;
            for(int i=0;i<tokens.size();i++) current_gen_args.push_back(tokens[i]);
        }
        std::vector<int> v_group_ids;
        for(const auto& it : current_groups){
            if(group_test_count.size()<=it)
                group_test_count.resize(it+1);
            ++group_test_count[it];
            v_group_ids.push_back(it);
        }
        if(v_group_ids.empty()){
            add_error(line_no, "Test does not belong to any subtasks");
            return false;
        }
        if(main_path.empty()){
            add_error(line_no, "No main source specified for this test");
            return false;
        }
        if(current_gen_args.empty()){
            add_error(line_no, "No generator arguments specified for this test");
            return false;
        }
        if(interactor_args.empty()){
            if(false){ /// TODO: de verificat daca problema e interactiva 
                add_error(line_no, "No interactor specified for this test");
                return false;
            }
        }
        if(val_args.empty()){
            add_warning(line_no, "No validator specified for this test");
        }
        if(checker_args.empty()){
            add_warning(line_no, "No checker specified for this test");
        }

        output.tests.push_back(test_metadata(main_path, current_gen_args, val_args, interactor_args, checker_args, v_group_ids));
        return true;
    };
    while(getline(fin,line)){
        //std::cout<<line_no<<' '<<line<<'\n';
        ++line_no;
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while(ss>>token) tokens.push_back(token);

        // elimin comentariile 
        for(int i=0;i<tokens.size();i++){
            size_t comment_pos=tokens[i].find("//");
            if(comment_pos==std::string::npos) continue;
            if(comment_pos){
                tokens[i].resize(comment_pos);
                tokens.resize(i+1);
            }
            else tokens.resize(i);
            break;
        }

        if(tokens.empty()) continue;
        if(tokens[0]=="#MAIN"){
            if(validate_main_args(line_no,tokens)){
                main_path=tokens[1];
            }
        } 
        else if(tokens[0]=="#VAL" || tokens[0]=="#VALIDATOR"){
            if(validate_validator_args(line_no,tokens)){
                val_args.clear();
                for(int i=1;i<tokens.size();i++)
                    val_args.push_back(tokens[i]);
            }
        }
        else if(tokens[0]=="#GEN" || tokens[0]=="#GENERATOR"){
            if(!cover_gen(line_no, tokens, gen_args)) continue;
        }
        else if(tokens[0]=="#CHECK" || tokens[0]=="#CHECKER"){
            if(validate_checker_args(line_no,tokens)){
                checker_args.clear();
                for(int i=1;i<tokens.size();i++)
                    checker_args.push_back(tokens[i]);
            }
        }
        else if(tokens[0]=="#INT" || tokens[0]=="#INTERACTOR"){
            if(validate_interactor_args(line_no,tokens)){
                interactor_args.clear();
                for(int i=1;i<tokens.size();i++)
                    interactor_args.push_back(tokens[i]);
            }
        }
        else if(tokens[0]=="#DEFGRP" || tokens[0]=="#DEFGROUP"){
            if(tokens.size()!=3){
                add_error(line_no, "Incorrect number of tokens");
                add_note(line_no, "Usage example: #DEFGRP 20.0 subtask_name");
                continue;
            }
            float pts=parse_float(tokens[1]);
            if(std::isnan(pts)){
                add_error(line_no, "Cannot convert '"+tokens[1]+"' to float");
                add_note(line_no, "Usage example: #DEFGRP 20.0 subtask_name");
                continue;
            }
            if(pts<0){
                add_error(line_no, "Point totals for subtasks must be non-negative");
                add_note(line_no, "Usage example: #DEFGRP 20.0 subtask_name");
                continue;
            }
            const std::string& name=tokens[2];
            if(group_ids.count(name)){
                add_error(line_no, "Group with name '"+name+"' has been defined previously");
                continue;
            }
            group_values[name]=pts;
            group_ids[name]=group_ids.size();
        }
        else if(tokens[0]=="#GROUP" || tokens[0]=="#GRP"){
            if(group_directive_gid>=0){
                current_groups.erase(group_directive_gid);
                group_directive_gid=-1;
            }
            if(tokens.size()!=2){
                add_error(line_no, "Incorrect number of tokens");
                add_note(line_no, "Usage example: #GROUP 20.0");
                continue;
            }
            float pts=parse_float(tokens[1]);
            if(std::isnan(pts)){
                add_error(line_no, "Cannot convert '"+tokens[1]+"' to float");
                add_note(line_no, "Usage example: #GROUP 20.0");
                continue;
            }
            if(pts<0){
                add_error(line_no, "Point totals for subtasks must be non-negative");
                add_note(line_no, "Usage example: #GROUP 20.0");
                continue;
            }
            int new_id=group_ids.size();
            group_directive_gid=new_id;
            current_groups.insert(new_id);
            group_values["__"+std::to_string(new_id)]=pts;
            group_ids["__"+std::to_string(new_id)]=group_ids.size();
        }
        else if(tokens[0]=="#ADDIN"){
            if(group_directive_gid>=0){
                current_groups.erase(group_directive_gid);
                group_directive_gid=-1;
            }
            if(tokens.size()==1){
                add_warning(line_no, "Redundant command, no group names specified");
                add_note(line_no, "Correct syntax: #ADDIN group_name1 group_name2 ...");
                continue;
            }
            for(int i=1;i<tokens.size();i++){
                if(!group_ids.count(tokens[i])){
                    add_error(line_no, "Unknown subtask name '"+tokens[i]+"'");
                    add_note(line_no, "Add the following line above: #DEFGRP 0.0 "+tokens[i]);
                    //goto failed_to_compile_line;
                }
                else{
                    int id=group_ids[tokens[i]];
                    if(current_groups.count(id)){
                        add_warning(line_no, "Tests below already belong to subtask '"+tokens[i]+"'");
                    }
                    else current_groups.insert(id);
                }
            }
        }
        else if(tokens[0]=="#NOTIN"){
            if(group_directive_gid>=0){
                current_groups.erase(group_directive_gid);
                group_directive_gid=-1;
            }
            if(tokens.size()==1){
                add_warning(line_no, "Redundant command, no group names specified");
                add_note(line_no, "Correct syntax: #NOTIN group_name1 group_name2 ...");
                continue;
            }
            for(int i=1;i<tokens.size();i++){
                if(!group_ids.count(tokens[i])){
                    add_error(line_no, "Unknown subtask name '"+tokens[i]+"'");
                    add_note(line_no, "Add the following line above: #DEFGRP 0.0 "+tokens[i]);
                    //goto failed_to_compile_line;
                }
                else{
                    int id=group_ids[tokens[i]];
                    if(!current_groups.count(id)){
                        add_warning(line_no, "Tests below already do not belong to subtask '"+tokens[i]+"'");
                    }
                    else current_groups.erase(id);
                }
            }
        }
        else if(tokens[0]=="#IN" || tokens[0]=="#SETIN"){
            if(group_directive_gid>=0){
                current_groups.erase(group_directive_gid);
                group_directive_gid=-1;
            }
            current_groups.clear();
            for(int i=1;i<tokens.size();i++){
                if(!group_ids.count(tokens[i])){
                    add_error(line_no, "Unknown subtask name '"+tokens[i]+"'");
                    add_note(line_no, "Add the following line above: #DEFGRP 0.0 "+tokens[i]);
                    //goto failed_to_compile_line;
                }
                else{
                    int id=group_ids[tokens[i]];
                    if(current_groups.count(id)){
                        add_warning(line_no, "Tests below already belong to subtask '"+tokens[i]+"'");
                    }
                    else current_groups.insert(id);
                }
            }
        }
        else if(tokens[0]=="#TEST"){
            if(group_directive_gid>=0){
                current_groups.erase(group_directive_gid);
                group_directive_gid=-1;
            }
            if(tokens.size()<3){
                add_error(line_no, "Incorrect number of tokens");
                add_note(line_no, "Correct syntax: #TEST 20.0 <generation parameters>");
                continue;
            }
            float pts=parse_float(tokens[1]);
            if(std::isnan(pts)){
                add_error(line_no, "Cannot convert '"+tokens[1]+"' to float");
                add_note(line_no, "Correct syntax: #TEST 20.0 <generation parameters>");
                continue;
            }
            if(pts<0){
                add_error(line_no, "Point totals for subtasks must be non-negative");
                add_note(line_no, "Correct syntax: #TEST 20.0 <generation parameters>");
                continue;
            }
            int new_id=group_ids.size();
            current_groups.insert(new_id);
            group_values["__"+std::to_string(new_id)]=pts;
            group_ids["__"+std::to_string(new_id)]=group_ids.size();
            std::vector<std::string> new_tokens(tokens.begin()+2, tokens.end());
            bool ok=add_test(line_no, new_tokens);
            current_groups.erase(new_id);
            if(!ok) continue;
        }
        else if(tokens[0][0]=='#'){
            add_error(line_no, "Unkown command '"+tokens[0]+"'");
            continue;
        }
        else{
            if(!add_test(line_no, tokens)) continue;
        }
    }
    for(const auto& it : group_values){
        output.groups.push_back(group_metadata(it.second, group_test_count[group_ids[it.first]], group_type_enum::GROUP_MIN));
        output.group_count++;
        output.total_points+=it.second;
    }
    output.test_count = output.tests.size();
    return this->failed ? result_enum::FAIL : result_enum::OK;
}

