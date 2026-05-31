#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

int lim = 999000;

int main(int argc, char *argv[])
{
    generator g(argc, argv);
    int c = 1;
    int n = (int)g.nextInt(10, lim);
    cout << c << '\n' << n << '\n';
    vector<int> vals;
    for(int i = 1; i <= n; i++)
        vals.push_back(i);
    g.shuffle(vals.begin(), vals.end());
    for(int i = 0; i < n; i++)
    {
        cout << vals[i];
        if(i + 1 < n) cout << ' ';
    }
    cout << '\n';
}
