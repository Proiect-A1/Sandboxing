#include "problem.h"
#include<bits/stdc++.h>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator inf;
    const int maxt=atoi(argv[1]);
    const int maxn=atoi(argv[2]);
    int t=inf.readInt(1, maxt);
    inf.readEoln();
    for(int i=0;i<t;i++){
        int n=inf.readInt(1, maxn);
        inf.readEoln();
        set<int> s;
        for(int j=0;j<n;j++){
            s.insert(inf.readInt(1,n));
            if(j==n-1) inf.readEoln();
            else inf.readSpace();
        }
        inf.failIf(s.size()!=n, "Array is not a permutation (testcase %d)", i+1);
    }
    inf.readEof();
    return 0;
}