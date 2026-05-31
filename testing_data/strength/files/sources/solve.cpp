#include <iostream>
using namespace std;

int main()
{
   int n;   cin >> n;

   int mx = 0, cntmx = 0;
   for (int i = 0; i < n; ++i)
   {
      int x;   cin >> x;
      if (x > mx) mx = x,  cntmx = 1;
      else if (x == mx) cntmx++;
   }

   cout << n - cntmx << '\n';
}