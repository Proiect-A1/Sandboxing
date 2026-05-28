#ifndef JSON_STATE_H
#define JSON_STATE_H

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

class json_state : public state
{
    private:
        string data;
        json j;

    public:
        void add() override;
        void execute() override;
        void next_state() override;
        json_state(int expected_length , state **founding_ptr , int fd) : state(expected_length , founding_ptr , fd) 
        {
            data.reserve(expected_length);
            for(int i = 0 ; i < expected_length ; i++) data += "1";
        }
};

#endif 