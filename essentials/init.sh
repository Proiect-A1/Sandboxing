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

 apt-get update \
	&& apt-get install -y --no-install-recommends \
		iptables acl openssl sudo \
		g++ zlib1g-dev libseccomp-dev libwebsockets-dev \
		debootstrap \
		unzip zip \
		ca-certificates \
        
rm -r sandbox 2> /dev/null
mkdir sandbox 2> /dev/null

debootstrap --variant=minbase questing ./sandbox http://archive.ubuntu.com/ubuntu/
#chroot ./sandbox apt-update
chroot ./sandbox apt-get install -y --no-install-recommends g++ rustc golang zlib1g-dev python3
chroot ./sandbox rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

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

rm get_dependencies.sh init_sandbox.sh
#rm fix_g++.sh 

mkdir etc
cp /etc/passwd etc
cp /etc/group etc