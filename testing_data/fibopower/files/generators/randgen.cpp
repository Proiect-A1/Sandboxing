#include "problem.h"
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

int n, k, q, nrgood;
map<ll, ll> fibopower;
vector<ll> fib, fpow;
ll valmax;
ll v[500005], s[500005], pmin[500005], lmax;

generator* gptr = nullptr;

void makefib()
{
    fib.push_back(1);
    fib.push_back(2);
    while (true) {
        ll lg = fib.size();
        lg--;
        ll val = fib[lg] + fib[lg - 1];
        if (val > 1000000000LL)
            break;
        fib.push_back(val);
    }
}

void makefibpow()
{
    sort(fib.begin(), fib.end());
    for (int i = 0; i < (int)fib.size(); i++)
        for (int j = i + 1; j < (int)fib.size() && fib[i] * fib[j] <= 1000000000LL; j++)
            for (int kk = j + 1; kk < (int)fib.size(); kk++)
                if (fib[i] * fib[j] <= 1000000000LL / fib[kk]) {
                    ll val = fib[i] * fib[j] * fib[kk];
                    if (val >= 1 && val <= valmax) {
                        fibopower[val] = 1;
                        fpow.push_back(val);
                    }
                }
    sort(fpow.begin(), fpow.end());
    fpow.erase(unique(fpow.begin(), fpow.end()), fpow.end());
}

ll getfibo()
{
    int poz = gptr->nextInt(0, (int)fpow.size() - 1);
    return fpow[poz];
}

ll getnonfibo()
{
    int nr = gptr->nextInt(1, (int)valmax);
    if (fibopower[nr])
        return getnonfibo();
    return nr;
}

pair<int, int> careless()
{
    int l = gptr->nextInt(1, n);
    int r = gptr->nextInt(l, n);
    return {l, r};
}

pair<int, int> careful()
{
    if (lmax < 1)
        return careless();
    int l = gptr->nextInt(1, (int)lmax);
    int r = gptr->nextInt((int)pmin[l], n);
    return {l, r};
}

int main(int argc, char* argv[])
{
    generator g(argc, argv);
    gptr = &g;

    n = atoi(argv[1]);
    q = atoi(argv[2]);
    k = atoi(argv[3]);
    valmax = atoi(argv[4]);
    nrgood = atoi(argv[5]);

    makefib();
    makefibpow();

    for (int i = 1; i <= nrgood; i++)
        v[i] = getfibo();
    for (int i = nrgood + 1; i <= n; i++)
        v[i] = getnonfibo();

    // Fisher-Yates shuffle (1-indexed)
    for (int i = n; i >= 1; i--) {
        int poz = gptr->nextInt(1, i);
        if (poz != i)
            swap(v[poz], v[i]);
    }

    for (int i = 1; i <= n; i++)
        s[i] = s[i - 1] + fibopower[v[i]];

    for (int i = 1; i <= n; i++) {
        int st = i;
        int dr = n;
        pmin[i] = n + 1;
        while (st <= dr) {
            int mij = (st + dr) / 2;
            if (s[mij] - s[i - 1] >= k) {
                pmin[i] = mij;
                dr = mij - 1;
            } else
                st = mij + 1;
        }
        if (pmin[i] <= n)
            lmax = i;
    }

    cout << n << ' ' << k << ' ' << q << endl;
    for (int i = 1; i <= n; i++) {
        cout << v[i];
        if (i + 1 <= n)
            cout << ' ';
    }
    cout << endl;
    cout << 1 << ' ' << n << endl;
    q--;
    while (q--) {
        if (gptr->nextInt(1, 10) == 10) {
            auto [l, r] = careless();
            cout << l << ' ' << r << endl;
        } else {
            auto [l, r] = careful();
            cout << l << ' ' << r << endl;
        }
    }
    return 0;
}
