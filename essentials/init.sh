#!/bin/bash 

if [ $(whoami) != "root" ]
then 
    echo "Needs root privileges"
    exit 2
fi

if [ $# -ne 1 ]
then 
    echo "Provide number of amarati"
    exit 1
fi

rm -r sandbox 2> /dev/null
mkdir sandbox 2> /dev/null

cp essentials/init_sandbox.sh ./sandbox
cp essentials/get_dependencies.sh ./sandbox

cd sandbox && bash init_sandbox.sh $1 
echo "
#include <bits/stdc++.h>
int main(){return 0;}
" > main.cpp 

g++ main.cpp -o main && bash get_dependencies.sh main
rm main.cpp main
rm get_dependencies.sh init_sandbox.sh

mkdir etc
cp /etc/passwd etc
cp /etc/group etc