#include <bits/stdc++.h>
using namespace std;
#define int long long
#define mp make_pair
#define ll long long
#define pb push_back
#define fi first
#define se second

vector<bool> prime;
vector<int> primes;

void genPrimes(const int LIM)
{
   prime.resize(LIM+1, 1);
   
   prime[0] = prime[1] = 0;
   for (int i = 2; i <= LIM; ++i)
      if (prime[i])
      {
         primes.pb(i);
         if (1LL*i*i > LIM)
            continue;
         
         for (int j = i*i; j <= LIM; j += i)
            prime[j] = 0;
      }
}


ll ezpow(ll a, ll b, ll mod)
{
   ll ans = 1;
   for (; b; b >>= 1)
   {
      if (b&1)
         ans = (__int128) ans * a % mod;
      a = (__int128) a * a % mod;
   }

   return ans;
}


bool prim(ll x) // https://en.wikipedia.org/wiki/Fermat_primality_test
{
   if (x == 1)
      return 0;
   if (x <= 3)
      return 1;
   
   ll a = 2 + rand() % (x-3);
   if (ezpow(a, x-1, x) == 1)
      return 1; // probabil prim
   return 0; // sigur neprim
}


int cntDiv(ll x)
{
   int cnt = 1;
   for (; x % 2 == 0; x >>= 1)
      cnt++;
   
   for (int d : primes)
   {
      int exp = 0;
      for (; x % d == 0; x /= d)
         exp++;
      
      cnt *= exp+1;
   }


   if (x != 1)
   {
      // avem 3 cazuri, x este format din a, din a*a sau din a*b. (a si b prime)
      if (prim(x))
         cnt *= 2;
      else if (sqrt(x) == (int) sqrt(x))
         cnt *= 3;
      else
         cnt *= 4;
   }

   return cnt;
}

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
int32_t main(){
    ll t; cin>>t; while(t--)
        tc();
}