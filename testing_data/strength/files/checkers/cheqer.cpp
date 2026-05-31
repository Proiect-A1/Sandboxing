#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
int main(int argc, char** argv){
    checker c(argc, argv);
    int n = c.in.readInt(1, 1e5);
    int okans = c.ok.readInt(0, n-1);
    int outans = c.out.readInt(0, n-1);
    if (okans == outans)    c.pquitf(OK, 1, "ok");
    c.pquitf(WA, 0, "different numbers");
    return 0;
}