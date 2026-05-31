#include <Server/json_length_state.h>
#include <Server/state.h>
#include <Server/json_state.h>

void json_length_state::add()
{
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

void json_length_state::execute()
{
    next_state();
}

void json_length_state::next_state()
{
    *founding_ptr = new json_state(data , founding_ptr , fd);
    (**founding_ptr).add();
    delete this;
}