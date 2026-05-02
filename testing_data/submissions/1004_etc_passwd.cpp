#include <bits/stdc++.h>
using namespace std;

int main()
{
    string passwd, line;
    
    ifstream fin ("/etc/passwd");
    if (!fin) {
        cerr << "NONO\n";
        return 0;
    }
    
    while (getline(fin, line)) {
        passwd += line + '\n';
    }
    
    cerr << passwd;
}
