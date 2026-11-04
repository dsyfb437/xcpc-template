/**
 * @brief 树的重心
 * @complexity O(n)
 * @index 点 1-based
 *
 * @idea 求树的重心。
 *       DFS 求子树大小；若每个儿子子树和补集都 ≤ n/2，该点就是重心。
 * @usage 例：`vector<int> c = tree_centroids(g);  // 1 个或 2 个`
 * @pitfall g 是无向树，空树返回空数组。返回 1-indexed 的点，按编号升序。
 *
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

inline vector<int> tree_centroids(const vector<vector<int>> &g) {
    int n = (int)g.size() - 1;
    if (n <= 0) return {};
    vector<int> par(n + 1, 0), sz(n + 1, 1);
    vector<int> order{1};
    for (int i = 0; i < n; i++) {
        int u = order[i];
        for (int v : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            order.push_back(v);
        }
    }
    for (int i = n - 1; i > 0; i--) {
        int u = order[i];
        sz[par[u]] += sz[u];
    }

    vector<int> res;
    for (int u = 1; u <= n; u++) {
        int mx = n - sz[u];
        for (int v : g[u])
            if (v != par[u])
                mx = max(mx, sz[v]);
        if (mx <= n / 2)
            res.push_back(u);
    }
    return res;
}
