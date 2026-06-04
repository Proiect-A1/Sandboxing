#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
typedef long double ld;
const ll NMAX=1e3,VMAX=1e6;
int n;
char mat[NMAX][NMAX];
struct bigInt{
    ll d[127]{}, Size=0;
    bigInt(){
        memset(d,sizeof d, 0);
        Size=0;
    }
};
bigInt& operator *= (bigInt& a, ll x){
    ll carry=0;
    for(ll i=0;i<a.Size;i++){
        a.d[i]=a.d[i]*x+carry;
        carry=a.d[i]>>59;
        a.d[i]&=(1ll<<59)-1;
        if(i+1==a.Size && carry>0) a.Size++;
    }
    return a;
}
bool operator < (const bigInt& a, const bigInt& b){
    if(a.Size!=b.Size) return a.Size<b.Size;
    for(int i=a.Size-1;i>=0;i--){
        if(a.d[i]!=b.d[i]) return a.d[i]<b.d[i];
    }
    return false;
}
bigInt readAndCheckAns(validator& v, checker &c){
    bigInt prod;
    prod.d[prod.Size++]=1;
    bool fnd[10]{};
    string s=v.readToken();
    if(s.size()!=n*2-2)
        c.pquitf(WA, 0, "Invalid string length", s.size(), n);
    int x=0,y=0;
    s.push_back('D');
    for(auto it : s){
        if(x>=n || y>=n){
            c.pquitf(WA, 0, "Invalid path");
        }
        ll val=mat[x][y]^48;
        for(ll i : {2,3,5,7})
            if(!fnd[i] && val%i==0)
                val=val/i*(i-1), fnd[i]=1;
        prod*=val;
        if(it=='D') x++;
        else if(it=='R') y++;
        else c.pquitf(WA, 0, "Invalid character %c", it);
    }
    return prod;
}
int main(int argc, char* argv[]){
    checker c(argc, argv);
    int maxt = atoi(argv[CHECKER_FIRST_ARG]);
    int maxn = atoi(argv[CHECKER_FIRST_ARG+1]);
    
    int t=c.in.readInt(1, maxt);
    c.in.readEoln();
    int score=0;
    for(int tid=1;tid<=t;tid++){
        n = c.in.readInt(2, maxn);
        c.in.readEoln();
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
                mat[i][j] = c.in.readChar("123456789");
            c.in.readEoln();
        }

        bigInt participant=readAndCheckAns(c.out, c);
        bigInt jury=readAndCheckAns(c.ok, c);
        if(participant<jury) c.pquitf(WA, 0, "Wrong answer");
    }
    c.pquitf(OK, 1, "Ok");
    return 0;
}
