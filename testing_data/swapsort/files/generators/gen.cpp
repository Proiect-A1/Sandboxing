#include "problem.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char** argv){
    generator g(argc, argv);
    int t=atoi(argv[1]);
    int n=atoi(argv[2]);
    cout<<t<<'\n';
    for(int i=0;i<t;i++){
        cout<<n<<'\n';
        vector<int> v;
        for(int i=0;i<n;i++) v.push_back(i+1);
        g.shuffle(v.begin(), v.end());
        for(int j=0;j<n;j++) cout<<v[j]<<" \n"[j==n-1];
    }
    return 0;
}