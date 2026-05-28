#include <Server/state.h>

state::state(int expected_length , state **founding_ptr , int fd)
{
    this -> expected_length = expected_length;
    this -> completed = 0;
    this -> founding_ptr = founding_ptr;
    this -> fd = fd;
}