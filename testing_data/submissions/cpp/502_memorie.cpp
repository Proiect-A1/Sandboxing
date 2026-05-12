#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 rng(33);
    int x;  cin >> x;
    
    // da RTE daca fac dimensiunea 0 (bun)
    vector<char> v(max(100, x * 10 * 1024 * 1024));
    
    // important e sa accesez fiecare pagina de memorie
    // merg din 100 in 100 ca sa fiu sigur, dar puteam si 2048 cred
    for (int i = 0; i < v.size(); i += 100) {
        v[i] = rng();
    }
    
    int ran = v[(uint32_t) rng() % v.size()];
    // cerr << "TEST " << x << " RAS " << ran << endl;
    cout << ran << '\n';
}
