#include <bits/stdc++.h>
using namespace std;

// dp[i][j] = suma pe care o obtinem din primele i elemente, inmultind doar cate j (1,2,3) elemente
// pentru ca nu este memorie, observam ca ne folosim doar de dp[i] si dp[i-1].

int n;
long long a[3 + 1];
long long b[3 + 1];

int main()
{
   cin >> n;
   for (int i = 1; i <= n; ++i)
   {
      int x;
      cin >> x;
      
      b[1] = a[1] + x;
      b[2] = a[2] + a[1]*x;
      b[3] = a[3] + a[2]*x;

      a[1] = b[1];
      a[2] = b[2];
      a[3] = b[3];
   }

   cout << a[3];
}