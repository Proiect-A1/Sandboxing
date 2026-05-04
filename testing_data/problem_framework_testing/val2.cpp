#include "validator.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    int maxt=atoi(argv[5]);
    validator v(argc, argv);
    int n=v.readInt(1,10);

    v.readEoln();
    for(int i=0;i<n;i++) v.readChar("qwertyuiopasdfghjklzxcvbnm");
    v.readEoln();

    for(int i=0;i<n;i++) v.readChar("qwertyuiopasdfghjklzxcvbnm");
    v.readEoln();
    v.readEof();
}