#include <bits/stdc++.h>
#include "header_helper.hpp"
#define BUFF_SIZE 4096

using namespace std;

namespace IOhelper 
{
    int current_pos = 0 , length = 0;
    char buff[BUFF_SIZE];

    int read_consistent(int fd , void *data , int len)
    {
        int total_read = 0;

        while(len)
        {
            int read_len = read(fd , data , len); if(read_len == -1) return -1;
            if(read_len == 0) return total_read;
            total_read += read_len;
            data = (void *)((long long)(data) + read_len);
            len -= read_len;
        }

        return total_read;
    }

    char get_char_fd(int fd)
    {
        if(current_pos == length)
        {
            length = read(fd , buff , BUFF_SIZE);
            if(length == -1) handle_error(1 , "get_char_fd()");
            if(length == 0) handle_error(1 , "get_char_fd()");
            current_pos = 0;
        }

        return buff[current_pos++];
    }

    void write_fd(int fd)
    {
        //to do
    }
}