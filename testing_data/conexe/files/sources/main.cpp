#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> vis(n + 1, false);
    int comp = 0;

    for (int s = 1; s <= n; s++) {
        if (vis[s]) continue;
        comp++;
        // DFS iterativ (evita stack overflow pe n mare)
        stack<int> st;
        st.push(s);
        vis[s] = true;
        while (!st.empty()) {
            int u = st.top();
            st.pop();
            for (int v : adj[u])
                if (!vis[v]) {
                    vis[v] = true;
                    st.push(v);
                }
        }
    }

    cout << comp << "\n";
    return 0;
}
