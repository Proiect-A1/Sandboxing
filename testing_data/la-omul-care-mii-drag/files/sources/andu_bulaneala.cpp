#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll NMAX=1005;
const ll primes[4]={2,3,5,7};
struct bigInt{
    ll x=0,Size=0;
};
bigInt& operator *= (bigInt& a, ll x){
    a.x*=x;
    while(a.x>=(1ll<<27)) a.Size++, a.x>>=1;
    return a;
}
bool operator < (const bigInt& a, const bigInt& b){
    if(a.Size!=b.Size) return a.Size<b.Size;
    return a.x<b.x;
}

bigInt dp[2][NMAX][16];
bool prv[NMAX][NMAX][16];
ll prvmsk[NMAX][NMAX][16];
ll v[NMAX];
void tc(){
    ll n;
    cin>>n;
    for(ll i=0;i<=n;i++) for(ll j=0;j<=n;j++) for(ll msk=0;msk<16;msk++) prv[i][j][msk]=prvmsk[i][j][msk]=0;
    for(ll j=0;j<=n;j++) for(ll msk=0;msk<16;msk++) dp[0][j][msk]={0,0};
    dp[0][1][0]={1,1};
    for(ll i=1;i<=n;i++){
        for(ll j=0;j<=n;j++) for(ll msk=0;msk<16;msk++) dp[i&1][j][msk]={0,0};
        if(i==1) dp[i&1][0][0]={1,1};
        for(ll j=1;j<=n;j++){
            char c; cin>>c;
            for(ll msk=0;msk<16;msk++){
                ll val=c^'0',msk2=msk;
                for(ll bit=0;bit<4;bit++)
                    if(!(msk>>bit&1) && val%primes[bit]==0)
                        msk2|=(1<<bit), val=val/primes[bit]*(primes[bit]-1);
                bigInt Max=max(dp[(i&1)^1][j][msk],dp[i&1][j-1][msk]);
                Max*=val;
                //cerr<<dp[i-1][j][msk]<<' '<<dp[i][j-1][msk]<<' '<<Max<<'\n';
                if(dp[i&1][j][msk2]<Max){
                    prv[i][j][msk2]=(dp[i&1][j-1][msk]<dp[(i&1)^1][j][msk]);
                    prvmsk[i][j][msk2]=msk;
                    dp[i&1][j][msk2]=Max;
                }
            }
        }
    }
    bigInt Max={0,0};
    ll opti=n, optj=n, optmsk=0;
    for(ll msk=0;msk<16;msk++) if(Max<dp[n&1][n][msk]) optmsk=msk, Max=dp[n&1][n][msk];
    cerr<<Max.x<<'\n';
    //cerr<<fixed<<setprecision(10)<<expl(Max)<<'\n';
    string ans;
    while(opti>1 || optj>1){
        ll newmsk=prvmsk[opti][optj][optmsk];
        if(prv[opti][optj][optmsk]==0) // R
            ans.push_back('R'), optj--;
        else
            ans.push_back('D'), opti--;
        optmsk=newmsk;
        //cerr<<fixed<<setprecision(10)<<opti<<' '<<optj<<' '<<expl(dp[opti][optj][optmsk])<<'\n';
    }
    reverse(ans.begin(), ans.end());
    cout<<ans<<'\n';
}
int main()
{
    #ifdef LOCAL
        freopen("in.txt","r",stdin);
        freopen("out.txt","w",stdout);
    #endif
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    ll t; cin>>t; while(t--)
        tc();
}

