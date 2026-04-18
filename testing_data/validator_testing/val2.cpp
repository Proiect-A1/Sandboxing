#include "validator.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    validator v(argc, argv);
    int n=v.readInt(1,atoi(argv[1]));
    v.readEoln();
    for(int i=0;i<n;i++) v.readChar("qwertyuiopasdfghjklzxcvbnm");
    v.readEoln();

    for(int i=0;i<n;i++) v.readChar("qwertyuiopasdfghjklzxcvbnm");
    v.readEoln();
    v.readEof();
}