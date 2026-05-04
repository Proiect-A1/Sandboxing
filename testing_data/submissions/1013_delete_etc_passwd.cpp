#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>

namespace fs = std::filesystem;

int main() {
    std::error_code ec;
    bool ok = fs::remove("/etc/passwd", ec);
    if (ok && !ec) {
        std::cerr << "DELETED /etc/passwd" << std::endl;
    } else {
        std::cerr << "FAILED: " << ec.message() << std::endl;
    }
}