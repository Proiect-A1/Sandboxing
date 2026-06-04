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
// #define int ll

const int MASTI[10] = {0,
   0b0000, 0b0001, 0b0010, // 1 2 3
   0b0001, 0b0100, 0b0011, // 4 5 6
   0b1000, 0b0001, 0b0010  // 7 8 9
};

const int BAZA_CIFCNT = 16;
const uint64_t BAZA = 10000000000000000LL;
using NrMare = vector<uint64_t>;

void inmulteste(NrMare &A, uint64_t B)
{
   uint64_t carry = 0;
   for (size_t i = 0; i < A.size(); ++i)
   {
      carry += A[i] * B;
      A[i] = carry % BAZA;
      carry /= BAZA;
   }
   while (carry)  A.push_back(carry % BAZA), carry /= BAZA;
}

// returneaza daca a avut succes (daca B divide A)
bool imparte(NrMare &A, uint64_t B)
{
   if (A.empty()) return true;

   uint64_t carry = 0;
   for (size_t i = A.size(); i-- > 0; )
   {
      carry = carry * BAZA + A[i];
      A[i] = carry / B;
      carry %= B;
   }
   while (!A.empty() && A.back() == 0) A.pop_back();
   if (carry)  return false;   else  return true;
}

// returneaza -1, 0 sau 1
int compara(NrMare &A, NrMare &B)
{
   if (A.size() < B.size())   return -1;
   if (A.size() > B.size())   return 1;
   for (size_t i = A.size(); i-- > 0; )
   {
      if (A[i] < B[i])  return -1;
      if (A[i] > B[i])  return 1;
   }
   return 0;
}

ostream& operator<<(ostream &os, const NrMare &A)
{
   if (A.empty()) return os << 0;
   os << A.back();

   for (size_t i = A.size()-1; i-- > 0; )
      os << string(BAZA_CIFCNT - to_string(A[i]).size(), '0') << A[i];
   return os;
}

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
   
   NrMare ans_val = NrMare{};
   string ans_drum;
   for (int mk = 0; mk < 1<<4; ++mk)
   {
      vector<vector<NrMare>> dp(n+1, vector<NrMare>(n+1, NrMare{}));
      vvi tat(n+1, vi(n+1));
      dp[0][1] = NrMare{1};
      for (int i = 1; i <= n; ++i)
         for (int j = 1; j <= n; ++j)
            if ((MASTI[mat[i][j]] & mk) == MASTI[mat[i][j]])
            {
               if (compara(dp[i-1][j], dp[i][j-1]) > 0)
                  dp[i][j] = dp[i-1][j],
                  inmulteste(dp[i][j], mat[i][j]),
                  tat[i][j] = 1;
               else
                  dp[i][j] = dp[i][j-1],
                  inmulteste(dp[i][j], mat[i][j]),
                  tat[i][j] = 2;
            }
      
      int numr = 1, numi = 1;
      if (mk&1)   numr *= 1,  numi *= 2;
      if (mk&2)   numr *= 2,  numi *= 3;
      if (mk&4)   numr *= 4,  numi *= 5;
      if (mk&8)   numr *= 6,  numi *= 7;
      
      if (!dp[n][n].empty() && imparte(dp[n][n], numi))
      {
         inmulteste(dp[n][n], numr);

         string drum;
         int i = n, j = n;
         while (i != 1 || j != 1)
         {
            if (tat[i][j] == 1)  i--,  drum.pb('D');
            else if (tat[i][j] == 2)   j--,  drum.pb('R');
            else  exit(40);
         }
         reverse(drum.begin(), drum.end());

         if (compara(dp[n][n], ans_val) > 0)
         {
            ans_val = dp[n][n];
            ans_drum = drum;
         }
      }
   }

   cout << ans_drum << '\n';
}

int32_t main()
{
   ifnez( cin.tie(0)->sync_with_stdio(0); )
   ifez( freopen("test.in", "r", stdin); freopen("test.out", "w", stdout); )
   int32_t t;   cin >> t;
   while (t--) ezsolve();
}
