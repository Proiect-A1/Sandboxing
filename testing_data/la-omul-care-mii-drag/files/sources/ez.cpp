// ______________________________________ v  EZ  v ______________________________________
#define mp make_pair
#define ll long long
#define pb push_back
#define fi first
#define se second
#define vi vector<int>
#define vvi vector<vi>
#define pii pair<int,int>
#define vpii vector<pii>
#define vvpii vector<vpii>
#define rg ranges::
#define wer | views::drop(1)
#define imax2 1073741823
#define imax 2147483647
#define lmax2 4611686018427387903
#define lmax 9223372036854775807
#ifdef EZ
   #define ifez(x) x
   #define ifnez(x)
   #ifdef EZDEBUG
      #include "ez/ezdebug.hpp"
   #else
      #include "ez/ez.hpp"
   #endif
   class I{public:const char*c;I(const char*c):c(c){}template<typename T>I&operator<<(const T&v){std::cerr<<c<<v<<"\033[0m";return*this;}I&operator<<(
   std::ostream&(*m)(std::ostream&)){std::cerr<<m;return*this;}};I cerw(""),cerre("\033[31m"),cerg("\033[32m"),cerb("\033[34m"),CERR("\u001b[37;1;46m");
   #define cerr cerre
#else
   #define ifez(x)
   #define ifnez(x) x
   #include <bits/stdc++.h>
   struct ezpz { template<typename T> ezpz& operator<<(const T&) { return *this; } };
   #define cerw ezpz()
   #define cerr ezpz()
   #define cerg ezpz()
   #define cerb ezpz()
   #define CERR ezpz()
#endif
using namespace std;

// ______________________________________ v CODE v ______________________________________
#pragma GCC target("avx2")
#define int ll
#define ld long double
const int mod = 1000000007;

const int MASTI[10] = {0,
   0b0000, 0b0001, 0b0010, // 1 2 3
   0b0001, 0b0100, 0b0011, // 4 5 6
   0b1000, 0b0001, 0b0010  // 7 8 9
};

void ezsolve()
{
   int n;   cin >> n;
   vvi mat(n+1, vi(n+1));
   for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= n; ++j)
      {
         char c;  cin >> c;
         mat[i][j] = c-'0';
      }
   
   pair<ld, string> bs;  bs.fi = -1e9;
   for (int mk = 0; mk < 1<<4; ++mk)
   {
      vector<vector<ld>> dp(n+1, vector<ld>(n+1, -1e9));
      vvi tat(n+1, vi(n+1));
      dp[0][1] = 0;
      for (int i = 1; i <= n; ++i)
         for (int j = 1; j <= n; ++j)
            if ((MASTI[mat[i][j]] & mk) == MASTI[mat[i][j]])
            {
               if (dp[i-1][j] == -1e9 && dp[i][j-1] == -1e9)   continue;

               if (dp[i-1][j] > dp[i][j-1])
                  dp[i][j] = dp[i-1][j] + log(mat[i][j]),
                  tat[i][j] = 1;
               else
                  dp[i][j] = dp[i][j-1] + log(mat[i][j]),
                  tat[i][j] = 2;
            }
      
      ld frac = 1;
      if (mk&1)   frac *= 1./2;
      if (mk&2)   frac *= 2./3;
      if (mk&4)   frac *= 4./5;
      if (mk&8)   frac *= 6./7;
      
      if (dp[n][n] != -1e9)
      {
         string drum;
         int i = n, j = n;
         while (i != 1 || j != 1)
         {
            if (tat[i][j] == 1)  i--,  drum.pb('D');
            else if (tat[i][j] == 2)   j--,  drum.pb('R');
            else  exit(40);
         }
         reverse(drum.begin(), drum.end());

         bs = max(bs, mp(dp[n][n] + log(frac), drum));
      }
   }

   ifez(cerw << "log is " << bs.fi << '\n';
   if (bs.se.size() != 2*n-2)
      cerr << "exp. size " << 2*n-2 << " got " << bs.se.size() << '\n';
   )
   
   cout << bs.se << '\n';
}

int32_t main()
{
   ifnez( cin.tie(0)->sync_with_stdio(0); )
   ifez( freopen("test.in", "r", stdin); freopen("test.out", "w", stdout); )
   int32_t t;   cin >> t;
   while (t--) ezsolve();
}
