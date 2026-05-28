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
            handle_error(1 , "json_length_state add()");
        }

        completed += rd;

        if(completed == expected_length)
        {
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