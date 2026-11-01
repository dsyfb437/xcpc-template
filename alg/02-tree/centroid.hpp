/**
 * @brief 树的重心
 * @complexity O(n)
 * @index 点 1-based
 *
 * 用途：求树的重心——删掉它以后，最大连通块最小的点。一般 1 个，最多 2 个。
 * 做法：DFS 求子树大小；若每个儿子子树和补集都 ≤ n/2，该点就是重心。
 * 递归实现；深链本地测试前先 `ulimit -s unlimited`。
 * 坑：g 必须连通；返回 1-indexed 的点，按编号升序。
 *
 * 例：
 *   vector<int> c = tree_centroids(g);  // 1 个或 2 个
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

inline vector<int> tree_centroids(const vector<vector<int>> &g) {
    int n = (int)g.size() - 1;
    vector<int> par(n + 1, 0), sz(n + 1, 1);
    auto dfs = [&](auto &&self, int u, int p) -> void {
        par[u] = p;
        sz[u] = 1;
        for (int v : g[u])
            if (v != p) {
                self(self, v, u);
                sz[u] += sz[v];
            }
    };
    dfs(dfs, 1, 0);

    vector<int> res;
    for (int u = 1; u <= n; u++) {
        int mx = n - sz[u];
        for (int v : g[u])
            if (v != par[u])
                mx = max(mx, sz[v]);
        if (mx * 2 <= n)
            res.push_back(u);
    }
    return res;
}
