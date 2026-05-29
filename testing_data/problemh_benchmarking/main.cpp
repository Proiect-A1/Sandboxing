#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
const ll NMAX=1e6+5;
ll v[NMAX];
int main(){
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    ll n;
    cin>>n;
    for(ll i=0;i<n;i++)
        cin>>v[i];
    sort(v,v+n);
    for(ll i=0;i<n;i++)
        cout<<v[i]<<' ';
}