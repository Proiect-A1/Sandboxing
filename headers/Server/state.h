#ifndef STATE_H
#define STATE_H

#include <bits/stdc++.h>
#include "header_helper.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <Chestii_cu_data/test_metadata.h>
#include <Chestii_cu_data/group_metadata.h>
#include <Enums/result_enum.h>

#define BUFF_SIZE 4096

using namespace std;

class state 
{
    protected:
        int expected_length;
        int completed;
        state **founding_ptr;
        int fd;

    public:
        virtual void add() = 0;
        virtual void execute() = 0;
        virtual void next_state() = 0;
        state(int expected_length , state **founding_ptr , int fd);
};

#endif 