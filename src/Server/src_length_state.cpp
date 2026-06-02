#include <Server/json_length_state.h>
#include <Server/state.h>
#include <Server/src_length_state.h>
#include <Enums/language_enum.h>
#include <Utilities/general_utilities.h>
#include <Utilities/architecture_utilities.h>
#include <Server/src_state.h>

void src_length_state::add()
{
    if(completed == expected_length)
    {
        LOG_ERROR(std::string("Invalid request received: "));
        *founding_ptr = nullptr;
        delete this;
        return;
    }

    while(1)
    {
        int rem = expected_length - completed;
        int rd = read(fd , &data + completed , rem);
        
        if(rd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            break;
        }
        else if(rd == -1)
        {
            LOG_ERROR(std::string("Connection lost"));
            *founding_ptr = nullptr;
            delete this;
            return;
        }
        else if(rd == 0)
        {
            LOG_ERROR(std::string("Invalid request received: "));
            *founding_ptr = nullptr;
            delete this;
            return;
        }

        completed += rd;

        if(completed == expected_length)
        {
            if(data <= 0)
            {
                LOG_ERROR(std::string("Invalid request received: "));
                *founding_ptr = nullptr;
                delete this;
                return;
            }
        
            execute();
            break;
        }
    }   
}

void src_length_state::execute()
{
    string submission_id = j["submissionId"].get < string > ();
    int rev_id = j["revId"].get < int > ();
    string problem_id = j["problemId"].get < string > ();
    language_enum language = general_utilities::string_to_language(j["language"].get < string > ());

    char path[PATH_MAX];
    sprintf(path , "%s/submissions/%s" , architecture_utilities::get_sandbox_path().c_str() , submission_id.c_str());
    if(mkdir(path , 0770) == -1) handle_error(1 , "mkdir() evaluate_request()");
    
    string source_path = architecture_utilities::get_submission_source_path(submission_id, language);
    submission_fd = open(source_path.c_str() , O_CREAT | O_TRUNC | O_RDWR , 0660); if(submission_fd == -1) handle_error(1 , "open() evaluate_request()");
  
    next_state();
}

void src_length_state::next_state()
{
    *founding_ptr = new src_state(data , founding_ptr , fd , j , submission_fd);
    (**founding_ptr).add();
    delete this;
}