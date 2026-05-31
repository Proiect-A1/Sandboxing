#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

/// input file-ul e stdin
/// argumentele din command line sunt date din tests.gen
int main(int argc, char** argv){
    validator inf;
    const int maxn = atoi(argv[1]);
    int c = (int)inf.readInt(1, 2);
    inf.readEoln();
    int n = (int)inf.readInt(1, maxn);
    inf.readEoln();
    set<int> s;
    for(int j = 0; j < n; j++){
        s.insert((int)inf.readInt(1, n));
        if(j == n-1) inf.readEoln();
        else inf.readSpace();
    }
    inf.failIf(s.size() != (size_t)n, "Array is not a permutation");
    inf.readEof();
    return 0;
}
