#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
int main(int argc, char** argv){
    generator g(argc, argv);
    ll n=atoi(argv[1]);
    cout<<1<<'\n';
    cout<<n<<'\n';
    for(ll i=0;i<n;i++){
        for(ll j=0;j<n;j++) cout<<9;
        cout<<'\n';
    }
    return 0;
}
