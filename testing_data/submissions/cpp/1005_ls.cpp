#include <bits/stdc++.h>
using namespace std;

int main()
{
    error_code ec;
    
    cerr << "STARTING:\n";
    cerr << filesystem::current_path() << endl;
    for (const auto &entry : filesystem::recursive_directory_iterator(
            ".",
            filesystem::directory_options::skip_permission_denied,
            ec
    )) {
        cerr << "- " << entry.path() << endl;
    }
    
    cerr << filesystem::current_path() << endl;
    for (const auto &entry : filesystem::recursive_directory_iterator(
            "/",
            filesystem::directory_options::skip_permission_denied,
            ec
    )) {
        cerr << "- " << entry.path() << endl;
    }
}
