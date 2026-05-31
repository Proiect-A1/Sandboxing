#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv)
{
    checker c(argc, argv);

    // Read n, k, q from input file to know how many answers to compare
    c.in.readInt(1, 100000); // n
    c.in.readInt(1, 100000); // k
    int q = (int)c.in.readInt(1, 100000); // q

    for (int i = 1; i <= q; i++) {
        long long j = c.ok.readInt(0, 5000000000LL);
        long long p = c.out.readInt(-1, 5000000001LL);
        if (j != p)
            c.pquitf(WA, 0.0, "%d-th answer differs: expected %lld, found %lld", i, j, p);
    }
    c.pquitf(OK, 1.0, "%d answers", q);
    return 0;
}
