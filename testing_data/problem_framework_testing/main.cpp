#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
const ll NMAX=105;
ll p[NMAX];
void tc(){
    vector<pll> ans;
    ll n; cin>>n;
    for(ll i=1;i<=n;i++)
        cin>>p[i];
    for(ll i=0;i<n;i++){
        while(p[i]!=i){
            ans.push_back({i, p[i]});
            swap(p[i], p[p[i]]);
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