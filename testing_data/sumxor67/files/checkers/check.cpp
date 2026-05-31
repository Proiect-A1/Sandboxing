#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
    checker c(argc, argv);
    int juryrez = (int)c.ok.readInt(0, 4000000);
    int contestantrez = (int)c.out.readInt(-2000000000LL, 2000000000LL);
    if(juryrez != contestantrez)
        c.pquitf(WA, 0.0, "The sum is wrong: expected %d, found %d", juryrez, contestantrez);
    int cerinta = (int)c.in.readInt(1, 2);
    int n = (int)c.in.readInt(1, 1000000);
    vector<int> a, b;
    map<int,int> f;
    for(int i = 1; i <= n; i++)
        a.push_back((int)c.in.readInt(1, n));
    for(int i = 1; i <= n; i++)
    {
        int x = (int)c.out.readInt(-2000000000LL, 2000000000LL);
        if(x < 1 || x > n)
            c.pquitf(WA, 0.0, "The values in the sequence are not between 1 and n");
        if(f[x] == 1)
            c.pquitf(WA, 0.0, "The output sequence is not a permutation");
        f[x] = 1;
        b.push_back(x);
    }
    int suma = 0;
    for(int i = 0; i < n; i++)
        suma ^= (a[i] + b[i]);
    if(suma != juryrez)
        c.pquitf(WA, 0.0, "The output sequence doesn't match the answer");
    c.pquitf(OK, 1.0, "OK");
    return 0;
}
