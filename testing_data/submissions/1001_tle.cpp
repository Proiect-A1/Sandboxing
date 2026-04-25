#include <bits/stdc++.h>
using namespace std;

const int nMAX = 100e3;

int n;
int v[nMAX + 1];

int main()
{
   cin >> n;
   for (int i = 1; i <= n; ++i)
      cin >> v[i];

   long long ans = 0;
   for (int i = 1; i <= n-2; ++i)
      for (int j = i+1; j <= n-1; ++j)
         for (int k = j+1; k <= n; ++k)
            ans += v[i] * v[j] * v[k];
   
   cout << ans;
}