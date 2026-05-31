#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
    generator g(argc, argv);
    const int maxn = atoi(argv[1]);
    int c = 1;
    int n = (int)g.nextInt(1, maxn);
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
