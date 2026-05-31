#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

ll n, k, q;
vector<ll> fib;
map<ll, ll> good;
ll s[100005], p[2][100005], sp[2][100005];

ll solve(ll l, ll r, ll ind)
{
    ll last = l - 1;
    ll st = l;
    ll dr = r;
    while (st <= dr) {
        ll mij = (st + dr) / 2;
        if (p[ind][mij] <= r) {
            last = mij;
            st = mij + 1;
        } else
            dr = mij - 1;
    }
    ll rez = (r + 1) * (last - l + 1) - (sp[ind][last] - sp[ind][l - 1]);
    return rez;
}

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
            for (int pp = j + 1; pp < (int)fib.size(); pp++)
                if (fib[i] * fib[j] <= 1e9 / fib[pp]) {
                    ll val = fib[i] * fib[j] * fib[pp];
                    good[val] = 1;
                }

    cin >> n >> k >> q;
    for (int i = 1; i <= n; i++) {
        ll x;
        cin >> x;
        s[i] = s[i - 1] + good[x];
    }
    for (int i = 1; i <= n; i++) {
        p[0][i] = n + 1;
        ll st = i;
        ll dr = n;
        while (st <= dr) {
            int mij = (st + dr) / 2;
            if (s[mij] - s[i - 1] >= k) {
                p[0][i] = mij;
                dr = mij - 1;
            } else
                st = mij + 1;
        }
        p[1][i] = n + 1;
        st = i;
        dr = n;
        while (st <= dr) {
            int mij = (st + dr) / 2;
            if (s[mij] - s[i - 1] >= k + 1) {
                p[1][i] = mij;
                dr = mij - 1;
            } else
                st = mij + 1;
        }
        sp[0][i] = sp[0][i - 1] + p[0][i];
        sp[1][i] = sp[1][i - 1] + p[1][i];
    }
    while (q--) {
        ll l, r;
        cin >> l >> r;
        ll ans = solve(l, r, 0) - solve(l, r, 1);
        cout << ans << '\n';
    }
    return 0;
}
