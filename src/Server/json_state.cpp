#include <Server/json_state.h>
#include <Server/state.h>
#include <Server/src_length_state.h>

void json_state::add()
{
    if(completed == expected_length)
    {
        return;
    }

    while(1)
    {
        int rem = expected_length - completed;
        int rd = read(fd , (void *) ((long long) data.c_str() + completed) , rem);
        
        if(rd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            break;
        }
        else if(rd == -1)
        {
            handle_error(1 , "json_state add()");
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
            execute();
            break;
        }
    }   
}

void json_state::execute()
{
    try
    {
        j = json::parse(data);
        LOG_DEBUG(std::string("Request string: ") + j.dump());

        if(!j.contains("request"))
        {
            LOG_ERROR("Invalid request received");
            *founding_ptr = nullptr;
            delete this;
            return;
        }

        next_state();
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(std::string("Invalid request received: ") + e.what());
        *founding_ptr = nullptr;
        delete this;
    }
}

void json_state::next_state()
{
    *founding_ptr = new src_length_state(sizeof(int) , founding_ptr , fd , j);
    (**founding_ptr).add();
    delete this;
}