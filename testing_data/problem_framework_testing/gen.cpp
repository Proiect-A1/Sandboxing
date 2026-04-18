#include "generator.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char** argv){
    generator g(argc, argv);
    int n=atoi(argv[1]);
    std::vector<int> v;
    int p[100];
    for(int i=0;i<n;i++) v.push_back(i),p[i]=i; 
    cout<<n<<' ';
    //cout<<g.nextString('l','a', 1)<<'\n';
    for(int i=0;i<n;i++)
        cout<<g.nextInt(1,atoi(argv[2]))<<" \n"[i==n-1];
    g.shuffle(v.begin(), v.end());
    g.shuffle(p,p+n);
    for(int i=0;i<n;i++)
        cout<<v[i]<<" \n"[i==n-1];
    for(int i=0;i<n;i++)
        cout<<p[i]<<" \n"[i==n-1];
    cout<<g.nextString("aaabc",n*2)<<'\n';
    cout<<g.nextString('a','c',n*2)<<'\n';
    return 0;
}