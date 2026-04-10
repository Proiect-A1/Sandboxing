#pragma once 
#include <iostream>
#include <exception>

class unfinished_request_exception : public std::exception 
{
    public:
        unfinished_request_exception();
        const char *what() const throw();
};