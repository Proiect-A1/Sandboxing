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

// doar face produsul maxim (ignora phi complet)
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
   
   vector<vector<ld>> dp(n+1, vector<ld>(n+1, -1e9));
   vvi tat(n+1, vi(n+1));
   
   dp[0][1] = 0;
   for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= n; ++j)
      {
         dp[i][j] = max(dp[i-1][j], dp[i][j-1]) + logl(mat[i][j]);
         tat[i][j] = dp[i-1][j] > dp[i][j-1];
      }
   
   int i = n, j = n;
   string ans;
   while (i != 1 || j != 1)
   {
      if (tat[i][j]) i--,  ans.pb('D');
      else  j--,  ans.pb('R');
   }
   reverse(ans.begin(), ans.end());
   cout << ans << '\n';
}

int32_t main()
{
   ifnez( cin.tie(0)->sync_with_stdio(0); )
   ifez( freopen("test.in", "r", stdin); freopen("test.out", "w", stdout); )
   int32_t t;   cin >> t;
   while (t--) ezsolve();
}
