#include "problem.h"
#include<bits/stdc++.h>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator inf;
    const int maxt=atoi(argv[1]);
    const int maxn=atoi(argv[2]);
    int t=inf.readInt(1,maxt),sumn=0;
    inf.readEoln();
    for(int tid=1;tid<=t;tid++){
        int n=inf.readInt(2,maxn);
        sumn+=n*n;
        inf.failIf(sumn>maxn*maxn, "sum of n^2 is too large");
        inf.readEoln();
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                char c=inf.readChar("123456789");
            }
            inf.readEoln();
        }
    }
    inf.readEof();
    return 0;
}