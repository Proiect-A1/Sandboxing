#!/bin/bash

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

echo "RUNNING TESTS..."
echo "SANDBOX FOUND IN: $SANDBOX_PATH"

cd runners
g++ -I ../headers test_main.cpp ../src/*/* -o test_main 
cd ..
./runners/test_main 127.0.0.1 6000 15
