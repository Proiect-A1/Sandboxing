#!/bin/bash 

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

bash init.sh 11 ~/.bashrc

#try again