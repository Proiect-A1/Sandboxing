#include <iostream>
#include <vector>

int main() {
    std::vector<int> data;
    data.reserve(1024 * 1024);
    for (int i = 0; i < 1024 * 1024; ++i) {
        data.push_back(i);
    }
    std::cout << "Hello from input.cpp" << std::endl;
    return 0;
}
