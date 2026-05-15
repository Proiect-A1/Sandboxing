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

debootstrap --variant=minbase questing ./sandbox http://archive.ubuntu.com/ubuntu/
chroot ./sandbox apt install g++ rustc golang dotnet-sdk-8.0 zlib1g-dev python3 -y

cp essentials/init_sandbox.sh ./sandbox
cp essentials/get_dependencies.sh ./sandbox
#cp essentials/fix_g++.sh ./sandbox

cd sandbox && bash init_sandbox.sh $1 
echo "
#include <bits/stdc++.h>
#include <zlib.h>
#include <thread>
#include <dlfcn.h>
int main(){ zlibVersion(); std::thread t([](){}); t.join(); dlopen(NULL, RTLD_NOW); return 0;}
" > main.cpp 

g++ main.cpp -lz -lpthread -ldl -lrt -o main && bash get_dependencies.sh main
rm main.cpp main

mkdir -p usr/bin
cp /usr/bin/python3 usr/bin/
bash get_dependencies.sh /usr/bin/python3
mkdir -p usr/lib/
cp -r /usr/lib/python3* usr/lib/ 2>/dev/null

rm get_dependencies.sh init_sandbox.sh
#rm fix_g++.sh 

mkdir etc
cp /etc/passwd etc
cp /etc/group etc