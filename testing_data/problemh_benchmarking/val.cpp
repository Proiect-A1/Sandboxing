#include "problem.h"
#include<bits/stdc++.h>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator inf;
    const int maxn=atoi(argv[1]);
    int n=inf.readInt(1, maxn);
    inf.readEoln();
    for(int i=0;i<n;i++){
        inf.readInt();
        if(i!=n-1) inf.readSpace();
        else inf.readEoln();
    }
    inf.readEof();
    return 0;
}