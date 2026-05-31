#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

ll n, k, q;
vector<ll> fib;
map<ll, ll> good;
ll v[100005];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    fib.push_back(1);
    fib.push_back(2);
    while (true) {
        ll lg = fib.size();
        ll val = fib[lg - 1] + fib[lg - 2];
        if (val > 1e9)
            break;
        fib.push_back(val);
    }
    sort(fib.begin(), fib.end());
    for (int i = 0; i < (int)fib.size(); i++)
        for (int j = i + 1; j < (int)fib.size() && fib[i] * fib[j] <= 1e9; j++)
            for (int p = j + 1; p < (int)fib.size(); p++)
                if (fib[i] * fib[j] <= 1e9 / fib[p]) {
                    ll val = fib[i] * fib[j] * fib[p];
                    good[val] = 1;
                }

    cin >> n >> k >> q;
    for (int i = 1; i <= n; i++) {
        ll x;
        cin >> x;
        v[i] = good[x];
    }
    while (q--) {
        ll l, r;
        cin >> l >> r;
        ll ans = 0;
        for (int i = l; i <= r; i++)
            for (int j = i; j <= r; j++) {
                int suma = 0;
                for (int pp = i; pp <= j; pp++)
                    suma += v[pp];
                if (suma > k)
                    break;
                if (suma == k)
                    ans++;
            }
        cout << ans << '\n';
    }
    return 0;
}
