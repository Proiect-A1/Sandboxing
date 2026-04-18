#include<Tasks/test_generation_script_compiler.h>
#include<Utilities/architecture_utilities.h>
#include<Utilities/general_utilities.h>
#include<fstream>
#include<sstream>
#include<map>

tgsct::tgsct(const std::string& problem_id, int rev_id){
    this->problem_id = problem_id;
    this->rev_id = rev_id;
}
bool tgsct::check_permissions(){
    return true; /// verific permisiunile in execute in mai multe locuri
}
void tgsct::add_fatal_error(const std::string& msg){
    compilation_message_list.push_back("Fatal error: " + msg);
}
void tgsct::add_error(int line, const std::string& msg){
    compilation_message_list.push_back("[Line "+std::to_string(line)+"] Error: " + msg);
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
result_enum tgsct::execute(pthread_t thread_id, int user_id){
    std::string problem_folder=architecture_utilities::get_problem_data_folder(problem_id, rev_id);
    if(!is_folder(problem_folder)){
        add_fatal_error("Problem folder does not exist");
        return result_enum::FAIL;
    }
    std::string script_path=architecture_utilities::get_problem_script_path(problem_id, rev_id);  
    if(!is_file(script_path)){
        add_fatal_error("Script file not found");
        return result_enum::FAIL;
    }
    std::ifstream fin(script_path.c_str());
    if(!fin.is_open()){
        add_fatal_error("Could not open script file");
        return result_enum::FAIL;
    }
    int line_no=0;
    int test_no=0;
    std::map<std::string, int> group_ids;
    std::vector<int> current_groups;
    std::string main_path;
    std::string gen_path;
    std::string val_path;
    std::string line;
    output=problem_metadata();
    while(fin.getline(line)){
        ++line_no;
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while(line>>token) tokens.push_back(token);
        if(tokens[0]=="#MAIN"){
            if(tokens.size()!=2){
                add_error(line_no, "Main executable name must not contain spaces; command line arguments are not allowed for main sources");
                add_note(line_no, "Usage example: #MAIN main")
                continue;
            }
            if(tokens[1].find_first_of('.')!=string::npos){
                add_error(line_no, "Main executable name must not contain a dot ('.'); do not use file extensions in the executable name");
                add_note(line_no, "Usage example: #MAIN main")
                continue;
            }
            if(!general_utilities::is_file(architecture_utilities::get_source_exec_path(problem_id, rev_id, tokens[1]))){
                add_error(line_no, "Main executable '"+tokens[1]+"' does not exist");
                continue;
            }
            if(!general_utilities::is_executable(architecture_utilities::get_source_exec_path(problem_id, rev_id, tokens[1]))){
                add_error(line_no, "Main executable '"+tokens[1]+"' does not exist");
                continue;
            }
        } 
        else if(tokens[0]=="#GEN"){

        }
        else{
            /// generator implicit 
        }
    }
}


void tgsct::print_log(pthread_t thread_id, int user_id, const std::string& message){
  fprintf(stdout, "\033[93m[LOG  ]\033[0m Test generation script compiler task running on thread %lu, with user %d: %s\n", thread_id, user_id, message.c_str());
}

void tgsct::print_error(pthread_t thread_id, int user_id, const std::string& message){
  fprintf(stderr, "\033[31m[ERROR]\033[0m Test generation script compiler task running on thread %lu, with user %d: %s\n", thread_id, user_id, message.c_str());
}

