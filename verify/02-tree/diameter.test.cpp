#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"

#include <bits/stdc++.h>
#include "../../alg/02-tree/diameter.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<pair<int, ll>>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        a++, b++;
        g[a].push_back({b, c});
        g[b].push_back({a, c});
    }
    auto [d, u, v] = tree_diameter(g, 1);

    vector<int> par(n + 1, 0);
    vector<char> vis(n + 1, 0);
    vector<int> st{u};
    vis[u] = 1;
    while (!st.empty()) {
        int x = st.back();
        st.pop_back();
        for (auto [y, w] : g[x])
            if (!vis[y]) {
                vis[y] = 1;
                par[y] = x;
                st.push_back(y);
            }
    }
    vector<int> path;
    for (int x = v;; x = par[x]) {
        path.push_back(x);
        if (x == u)
            break;
    }
    reverse(path.begin(), path.end());

    cout << d << " " << path.size() << "\n";
    for (int i = 0; i < (int)path.size(); i++)
        cout << path[i] - 1 << " \n"[i + 1 == (int)path.size()];
}
