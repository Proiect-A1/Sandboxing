#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
const int NMAX=105;

int main(int argc, char** argv){
    checker c(argc, argv);
    int n=c.in.readInt();
    for(int i=0;i<n;i++){
        if(c.out.readInt()!=c.ok.readInt()){
            c.quitf(WA, "wrong answer");
        }
    }
    c.quitf(OK, "ok");
    return 0;
}