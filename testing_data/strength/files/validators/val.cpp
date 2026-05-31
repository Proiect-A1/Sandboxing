#include "problem.h"
#include<bits/stdc++.h>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator inf;
    int n=inf.readInt(1, 100000);
    inf.readEoln();
    for(int i=0;i<n;i++){
        int val=inf.readInt(1, 1000000000);
        if (i==n-1) inf.readEoln();
        else    inf.readSpace();
    }
    inf.readEof();
    return 0;
}
