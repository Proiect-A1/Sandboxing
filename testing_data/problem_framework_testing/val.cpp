#include "validator.h"
#include<iostream>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator v(argc, argv);
    int n=v.readInt(atoi(argv[1]), atoi(argv[2]));
    v.readSpace();
    int m=v.readInt(atoi(argv[3]), atoi(argv[4]));
    std::cout<<n<<' '<<m<<'\n';
    v.readEoln();
    v.readEof();
}