#include "problem.h"
#include <bits/stdc++.h>

using namespace std;

int lim = 999000;
int c;
int n;

int main(int argc, char *argv[])
{
    generator g(argc, argv);
    c = 2;
    n = (int)g.nextInt(100, lim);
    cout << c << endl << n << endl;
    vector<int> vals;
    for(int i = 1; i <= n; i++)
        vals.push_back(i);
    g.shuffle(vals.begin(), vals.end());
    for(int i = 0; i < (int)vals.size(); i++)
    {
        cout << vals[i];
        if(i + 1 < (int)vals.size())
            cout << ' ';
    }
    cout << endl;
}
