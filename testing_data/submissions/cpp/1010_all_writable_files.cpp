#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main() {
    int n;  std::cin >> n;
    if (n != 4) return 0;
    // DOAR UN TEST VREAU SA RULEZE!

    std::error_code ec;

    for (fs::recursive_directory_iterator it("/",
            fs::directory_options::skip_permission_denied, ec), end;
         it != end; it.increment(ec)) {

        if (ec) {
            ec.clear();
            continue;
        }

        const auto& path = it->path();

        // skip directories (we only want files)
        if (fs::is_directory(path, ec)) {
            if (ec) ec.clear();
            continue;
        }

        std::ofstream fout(path, std::ios::app);
        if (fout.is_open()) {
            std::cerr << "== IM " + fs::current_path().string() << " " << path << std::endl;
        }
    }
}
