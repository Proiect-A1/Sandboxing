#!/bin/bash

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

echo "SANDBOX FOUND IN: $SANDBOX_PATH"
cd $SANDBOX_PATH
#mkdir -p $SANDBOX_PATH/bin ; sudo cp /bin/bash $SANDBOX_PATH/bin ; sudo bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/bin/bash
#mkdir -p $SANDBOX_PATH/usr/bin ; sudo cp /usr/bin/ls $SANDBOX_PATH/usr/bin/ ; sudo bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/usr/bin/ls
bash get_dependencies.sh runs/amarat1/main_exec 

cd ../runners
g++ -I ../headers main.cpp ../src/*/* -o main && ./main 127.0.0.1 6000 15
