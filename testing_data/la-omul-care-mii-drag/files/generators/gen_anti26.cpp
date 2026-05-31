#include "problem.h"
#include<bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;
const ll NMAX=1005;
//22 12 31 37
//114 29 10 12
// 5 6 7 => 4 * 2 * 6 (4 de 2 si un 3)
// 18 11 31 37
// 110 28 10 12
char Ans[NMAX][NMAX];
int main(int argc, char** argv){
    generator g(argc, argv);
    ll n=1000;
    cout<<"1\n"<<"1000\n";
    vector<ll> a={18,11,31,37};
    vector<ll> b={110,28,10,12};
    for(ll i=0;i<n;i++) for(ll j=0;j<n;j++) Ans[i][j]='1';
    Ans[0][0]='5';
    Ans[n-1][n-1]='7';
    Ans[n-2][n-1]='6';
    Ans[n-1][n-2]='6';
    for(ll i=1;i<n;i++){
        if(a[0]>=3) Ans[0][i] = '8', a[0]-=3;
        else if(a[0]>=2) Ans[0][i] = '4', a[0]-=2;
        else if(a[0]>=1) Ans[0][i] = '2', a[0]-=1;
        else if(a[1]>=2) Ans[0][i] = '9', a[1]-=2;
        else if(a[1]>=1) Ans[0][i] = '3', a[1]-=1;
        else if(a[2]>=1) Ans[0][i] = '5', a[2]-=1;
        else if(a[3]>=1) Ans[0][i] = '7', a[3]-=1;
    }
    for(ll i=1;i<n;i++){
        if(b[0]>=3) Ans[i][0] = '8', b[0]-=3;
        else if(b[0]>=2) Ans[i][0] = '4', b[0]-=2;
        else if(b[0]>=1) Ans[i][0] = '2', b[0]-=1;
        else if(b[1]>=2) Ans[i][0] = '9', b[1]-=2;
        else if(b[1]>=1) Ans[i][0] = '3', b[1]-=1;
        else if(b[2]>=1) Ans[i][0] = '5', b[2]-=1;
        else if(b[3]>=1) Ans[i][0] = '7', b[3]-=1;
    }
    for(ll i=0;i<n;i++){
        for(ll j=0;j<n;j++)
            cout<<Ans[i][j];
        cout<<'\n';
    }
    return 0;
}
