#!/bin/bash 

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

if [ $# -ne 2 ]
then 
    echo "Provide number of amarati and the .rc file"
    exit 1
fi

mkdir sandbox 2> /dev/null
cp init_sandbox.sh ./sandbox
cp get_dependencies.sh ./sandbox
#g++ main.cpp src/* -o main;
#g++ client.cpp src/* -o client;

cd sandbox && bash init_sandbox.sh $1 $2
