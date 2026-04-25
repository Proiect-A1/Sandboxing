#include <bits/stdc++.h>
using namespace std;

const int WAVES = 100;
const int ELM_COUNT = 20'000'000;

int main() {
    mt19937 rng(33);
    
    int x;  cin >> x;
    ofstream gunoi("gunoi");
    
    for (int wave = 0; wave < WAVES/2; ++wave) {
        vector<char> v(ELM_COUNT);
        for (int i = 0; i < ELM_COUNT; i += 100) {
            v[i] = rng();
        }
        
        for (int g = 0; g < 20; ++g) {
            gunoi << v[rng() % ELM_COUNT];
        }
    }
    for (int wave = WAVES/2; wave < WAVES; ++wave) {
        char* v = new char[ELM_COUNT];
        for (int i = 0; i < ELM_COUNT; i += 100) {
            v[i] = rng();
        }
        
        for (int g = 0; g < 20; ++g) {
            gunoi << v[rng() % ELM_COUNT];
        }
        delete[] v;
    }
    
    cout << x << '\n';
    gunoi.close();
}
