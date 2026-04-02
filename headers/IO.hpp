#include <bits/stdc++.h>
#include "header_helper.hpp"
#define BUFF_SIZE 4096

using namespace std;

namespace IOhelper 
{
    int current_pos = 0 , length = 0;
    char buff[BUFF_SIZE];

    char get_char_fd(int fd)
    {
        if(current_pos == length)
        {
            length = read(fd , buff , BUFF_SIZE);
            if(length == -1) handle_error(1 , "get_char_fd()");
        }

        return buff[current_pos++];
    }

    void write_fd(int fd)
    {
        //to do
    }
}