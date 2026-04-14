#include <Server/exceptions.hpp>

const char *unfinished_request_exception::what() const throw()
{
    return "unfinished request";
}

unfinished_request_exception::unfinished_request_exception(){}