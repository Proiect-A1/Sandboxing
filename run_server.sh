#!/bin/bash

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

echo "SANDBOX FOUND IN: $SANDBOX_PATH"
cd $SANDBOX_PATH



cd ../runners
g++ -I ../headers main.cpp ../src/*/* -o main -lseccomp -lwebsockets
cd ..

echo $1 

if [ $# -eq 1 -a $1 == 'server' ]
then 
    ./runners/main 10.8.0.3 6000 15
elif [ $# -eq 1 -a $1 == 'local' ]
then
    ./runners/main 10.8.0.2 6000 15
elif [ $# -eq 1 -a $1 == 'vps' ]
then
    ./runners/main 10.0.0.240 6000 15
else
    ./runners/main 0.0.0.0 6000 15
fi
