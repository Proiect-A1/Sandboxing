#include <bits/stdc++.h>
using namespace std;

int main() {
    random_device rd;
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count() + rd());
    
    exit(rng());
}
