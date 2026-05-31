#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv)
{
    validator inf;
    const int maxn = atoi(argv[1]);
    const long long maxval = atoll(argv[2]);

    int n = (int)inf.readInt(1, maxn);
    inf.readSpace();
    (void)inf.readInt(1, n); // k in [1, n]
    inf.readSpace();
    int q = (int)inf.readInt(1, maxn);
    inf.readEoln();

    for (int i = 1; i <= n; i++) {
        inf.readInt(1, maxval);
        if (i < n)
            inf.readSpace();
    }
    inf.readEoln();

    for (int i = 1; i <= q; i++) {
        int l = (int)inf.readInt(1, n);
        inf.readSpace();
        inf.readInt(l, n); // r >= l
        inf.readEoln();
    }

    inf.readEof();
    return 0;
}
