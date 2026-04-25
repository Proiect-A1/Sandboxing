cd runners
g++ -I ../headers client_read.cpp ../src/*/* -o client && ./client 127.0.0.1 6000
