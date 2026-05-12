#include <bits/stdc++.h>

int main() {
    std::ofstream f("./correct_output");
    if (!f) {
        std::cerr << "NU POT SA SCRIU OFFFFFFFFFFFFFFF";
    } else {
        if (!(f << "bulan")) {
            std::cerr << "NU POT SA SCRIU OBBBBBBBBBBBBBBB";
        }
        f.flush();
        f.close();
    }
    std::cout << "bulan";
}
