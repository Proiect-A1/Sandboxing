#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
const ll NMAX=1e6+1;

int main(int argc, char** argv){
    generator g(argc, argv);
    ll t=atoi(argv[1]), n=atoi(argv[2]);
    cout<<t<<'\n';
    while(t--){
        cout<<n<<'\n';
        for(ll i=0;i<n;i++){
            for(ll j=0;j<n;j++) cout<<g.nextInt(1,9);
            cout<<'\n';
        }
    }
    return 0;
}

