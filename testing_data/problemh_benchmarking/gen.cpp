#include "problem.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char** argv){
    generator g(argc, argv);
    int n=atoi(argv[1]);
    cout<<n<<'\n';
    for(int i=0;i<n;i++)
        cout<<g.nextInt(LLONG_MIN, LLONG_MAX)<<" \n"[i==n-1];
    return 0;
}