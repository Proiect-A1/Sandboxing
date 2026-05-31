#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
const ll NMAX=1e6+1;
ll ns[NMAX];
int main(int argc, char** argv){
    ll n=atoi(argv[1]);
    cout<<"1\n"<<n<<'\n';
    for(ll i=0;i<n;i++){
        for(ll j=0;j<n;j++){
            if(i+j==n-1){
                if(i*2==n) cout<<'8';
                else cout<<'7';
            }
            else cout<<'9';
        }
        cout<<'\n';
    }
    return 0;
}
