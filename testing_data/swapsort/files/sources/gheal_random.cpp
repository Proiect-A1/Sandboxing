#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
ll rndnext(ll l, ll r){
    uniform_int_distribution<ll> rnd(l,r);
    return rnd(gen);
}
const ll NMAX=105;
ll p[NMAX];
void tc(){
    vector<pll> ans;
    ll n; cin>>n;
    for(ll i=1;i<=n;i++)
        cin>>p[i];
    for(ll iter=0;iter<1e5;iter++){
        ll i=rndnext(1,n);
        ll j=rndnext(1,n);
        if(i>j) swap(i,j);
        if(p[i]>p[j]){
            ans.push_back({i,j});
            swap(p[i], p[j]);
        }
    }
    cout<<ans.size()<<'\n';
    for(auto it : ans) cout<<it.first<<' '<<it.second<<'\n';
}
int main(){
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    ll t; cin>>t; while(t--)
        tc();
}