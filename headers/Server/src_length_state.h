#ifndef SRC_LENGTH_STATE_H
#define SRC_LENGTH_STATE_H

#include <bits/stdc++.h>
#include "header_helper.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <Chestii_cu_data/test_metadata.h>
#include <Chestii_cu_data/group_metadata.h>
#include <Enums/result_enum.h>
#include <Server/state.h>
#define BUFF_SIZE 4096

using namespace std;

class src_length_state : public state
{
    private:
        int data;
        json j;
        int submission_fd;
        
    public:
        void add() override;
        void execute() override;
        void next_state() override;
        src_length_state(int expected_length , state **founding_ptr , int fd , json j) : state(expected_length , founding_ptr , fd) , j(j) {}
};

#endif 