#include <Server/json_length_state.h>
#include <Server/state.h>
#include <Server/src_state.h>
#include <Singletoni/submission_manager.h>

void src_state::add()
{
    while(1)
    {
        int rem = expected_length - completed;
        int rd = read(fd , buff , min(rem , BUFF_SIZE));
        
        if(rd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            break;
        }
        else if(rd == -1 && (errno == ECONNRESET || errno == ETIMEDOUT || errno == EINTR || errno == ENOTCONN || errno == ECONNREFUSED))
        {
            LOG_ERROR(std::string("Connection lost"));
            *founding_ptr = nullptr;
            delete this;
            return;
        }
        else if(rd == -1)
        {
            handle_error(1 , "src_state add()");
        }
        else if(rd == 0)
        {
            LOG_ERROR(std::string("Invalid request received: "));
            *founding_ptr = nullptr;
            delete this;
            return;
        }

        if(write(submission_fd , buff , rd) != rd) handle_error(1 , "src_state write()");

        completed += rd;

        if(completed == expected_length)
        {
            execute();
            break;
        }
    }   
}

void src_state::execute()
{
    close(submission_fd);

    string submission_id = j["submissionId"].get < string > ();
    int rev_id = j["revId"].get < int > ();
    string problem_id = j["problemId"].get < string > ();
    language_enum language = general_utilities::string_to_language(j["language"].get < string > ());
    string url_download = j["downloadLink"].get < string > ();
    string url_upload = j["uploadLink"].get < string > ();


    submission_manager& sm = submission_manager::get_instance();

    sm.insert(submission_id, language, problem_id , rev_id , url_download , url_upload , fd);

    submission_data submission = sm.get_submission(submission_id);

    evaluator_task* eva = new evaluator_task(submission_id , problem_id , rev_id);

    task_queue::get_instance().push(eva);
    next_state();
}

void src_state::next_state()
{
    *founding_ptr = nullptr;
    LOG_DEBUG("                         SRC STATE ");
    delete this;
}