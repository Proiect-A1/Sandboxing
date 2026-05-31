#include "problem.h"
#include<bits/stdc++.h>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator inf;
    const int64_t vmax = 2'000'000'000;
    inf.readInt(-vmax, vmax);
    inf.readSpace();
    inf.readInt(-vmax, vmax);
    inf.readEoln();
    inf.readEof();
    return 0;
}