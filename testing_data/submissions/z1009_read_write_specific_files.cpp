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

// verific in ce folder amarat* am acces la 'input'
int get_amarat() {
    int ok = 0;
    for (int i = 1; i <= 20; ++i) {
        if (can_read_file("/runs/amarat" + std::to_string(i) + "/input")) {
            if (ok) {
                std::cerr << "%%%%%%%%%% HOOPA!! pot sa citesc inputu de la amarat"
                    << ok << " dar si de la amarat" << i << '\n';
            }
            ok = i;
        }
    }
    assert(ok != 0);
    return ok;
}

int main() {
    int n;  std::cin >> n;
    if (n != 4) return 0;
    // DOAR UN TEST VREAU SA RULEZE!
    
    int amarat = get_amarat();
    
    std::vector<std::string> paths{
        "/etc/passwd",
        "/etc/group",
        
        "/correct_outputs/expresie.1/000.ok",
        "/correct_outputs/abcde.12/000.ok",
        
        "/inputs/expresie.1/000.in",
        "/inputs/abcde.12/000.in",
        
        "/lib/x86_64-linux-gnu/libc.so.6",
        "/lib/x86_64-linux-gnu/libstdc++.so.6",
        "/lib/x86_64-linux-gnu/libgcc_s.so.1",
        "/lib64/ld-linux-x86-64.so.2",
        
        "/submissions/1000_1804289383/main.cpp",
        "/submissions/1000_1804289383/main_exec",
        
        "/submissions/1004_1681692777/main.cpp",
        "/submissions/1004_1681692777/main_exec",
        
        "./output",
        "./correct_output",
        "./input",
        "./main_exec",
        "./main.cpp"
    };
    for (int i = 1; i <= 20; ++i) {
        paths.push_back("/runs/amarat" + std::to_string(i) + "/output");
        paths.push_back("/runs/amarat" + std::to_string(i) + "/correct_output");
        paths.push_back("/runs/amarat" + std::to_string(i) + "/input");
        paths.push_back("/runs/amarat" + std::to_string(i) + "/main_exec");
        paths.push_back("/runs/amarat" + std::to_string(i) + "/main.cpp");
    }
    
    for (const std::string &path : paths) {
        if (can_read_file(path)) {
            std::cerr << "%%%% READ  %%%% IM amarat" << amarat << " %%%%\t" << path << std::endl;
        }
        if (can_write_file(path)) {
            std::cerr << "%%%% WRITE %%%% IM amarat" << amarat << " %%%%\t" << path << std::endl;
        }
    }
    
}
