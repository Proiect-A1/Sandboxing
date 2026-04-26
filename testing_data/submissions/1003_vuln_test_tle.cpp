#include <bits/stdc++.h>
using namespace std;

int main()
{
    cerr << "alex" << endl;
    cout << "liviu" << endl;
    ofstream("tralalero") << "tralala" << endl;
    
    volatile uintptr_t x = 0;
    while (1) {
        x += reinterpret_cast<uintptr_t>(&cerr);
    }
    
    cout << x << endl;
}
