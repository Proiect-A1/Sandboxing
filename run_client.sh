
g++ -I headers runners/client.cpp src/*/* -o runners/client -lseccomp -lwebsockets 


if [ $# -eq 1 -a $1 == 'server' ]
then 
     ./runners/client 10.8.0.3 6000
else 
    ./runners/client 127.0.0.1 6000
fi