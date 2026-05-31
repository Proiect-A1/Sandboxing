#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
const ll NMAX=1e6+1;
ll ns[NMAX];
int main(int argc, char** argv){
    generator g(argc, argv);
    ll t=atoi(argv[1]), sumn=atoi(argv[2]);
    cout<<t<<'\n';
    for(ll i=0;i<t;i++) ns[i]=2,sumn-=4;
    for(ll i=0;i<1e7;i++){
        ll pos=g.nextInt(t);
        if(sumn-2*ns[pos]-1>=0){
            sumn-=2*ns[pos]+1;
            ns[pos]++;
        }
    }
    for(ll tid=0;tid<t;tid++){
        ll n=ns[tid];
        cout<<n<<'\n';
        for(ll i=0;i<n;i++){
            for(ll j=0;j<n;j++) cout<<g.nextInt(1,9);
            cout<<'\n';
        }
    }
    return 0;
}
