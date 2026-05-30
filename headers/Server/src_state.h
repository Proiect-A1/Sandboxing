#ifndef SRC_STATE_H
#define SRC_STATE_H

#include <bits/stdc++.h>
#include <Server/header_helper.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <Chestii_cu_data/test_metadata.h>
#include <Chestii_cu_data/group_metadata.h>
#include <Enums/result_enum.h>
#include <Server/state.h>
#define BUFF_SIZE 4096

using namespace std;

class src_state : public state
{
    private:
        unsigned char buff[BUFF_SIZE];
        int submission_fd;
        json j;

    public:
        void add() override;
        void execute() override;
        void next_state() override;
        src_state(int expected_length , state **founding_ptr , int fd , json j , int submission_fd) : state(expected_length , founding_ptr , fd) , j(j) , submission_fd(submission_fd) {}
};

#endif 