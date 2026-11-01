#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include <bits/stdc++.h>
#include "../../alg/02-tree/lca.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<vector<int>> g(n + 1);
    for (int v = 1; v < n; v++) {
        int p;
        cin >> p;
        g[p + 1].push_back(v + 1);
        g[v + 1].push_back(p + 1);
    }
    LCA lca(g, 1);
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lca.lca(u + 1, v + 1) - 1 << '\n';
    }
}
