#include <bits/stdc++.h>
using namespace std;

int main() {
    random_device rd;
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count() + rd());
    
    int code = rng();
    cerr << "SHOULD EXIT " << code%255 << '\n';
    exit(code);
}
