#include <bits/stdc++.h>

int main() {
    std::ofstream f("mytmp");
    f << 5;
    f.close();
    
    std::ifstream fi("mytmp");
    int x;  fi >> x;
    fi.close();
    
    std::cout << x << '\n';
}
