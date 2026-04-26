#include <bits/stdc++.h>
using namespace std;

int n;
int vf[60 + 1];

int main()
{
   cin.tie(0)->sync_with_stdio(0);
   
   cin >> n;
   for (int i = 1; i <= n; ++i)
   {
      int x;
      cin >> x;
      vf[x + 30]++;
   }

   long long ans = 0;

// cazul 1: toate cele 3 numere egale. C(vf[i+30], 3) * i*i*i
   for (int i = -30; i <= 30; ++i)
      ans += vf[i+30] * (vf[i+30]-1) * (vf[i+30]-2) / 6 * i * i * i;

// cazul 2: 2 numere egale. C(vf[i+30], 2) * vf[j+30] * i*i*j
   for (int i = -30; i <= 30; ++i)
      for (int j = -30; j <= 30; ++j)
         if (i != j)
            ans += vf[i+30] * (vf[i+30]-1) / 2 * vf[j+30] * i * i * j;

// cazul 3: toate cele 3 numere diferite.
   for (int i = -30; i <= 28; ++i)
      for (int j = i+1; j <= 29; ++j)
         for (int k = j+1; k <= 30; ++k)
            ans += vf[i+30] * vf[j+30] * vf[k+30] * i * j * k;
   
   cout << ans;
}