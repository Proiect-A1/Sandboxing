#!/bin/bash

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

cd $SANDBOX_PATH
echo $SANDBOX_PATH
#sudo mkdir -p $SANDBOX_PATH/bin ; sudo cp /bin/bash $SANDBOX_PATH/bin ; sudo bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/bin/bash
#sudo mkdir -p $SANDBOX_PATH/usr/bin ; sudo cp /usr/bin/ls $SANDBOX_PATH/usr/bin/ ; sudo bash $SANDBOX_PATH/get_dependencies.sh $SANDBOX_PATH/usr/bin/ls
#sudo bash get_dependencies.sh runs/amarat1/main_exec 

cd ../

g++ -I headers main.cpp src/*/* -o main && ./main 10.8.0.3 6000 15
