/**
 * @brief 树上路径交
 * @complexity 单次 O(log n)
 * @index 点 1-based
 *
 * 用途：求两条路径 (a,b)、(c,d) 的交。返回 {交的边数, 端点 x, 端点 y}；
 * 无交返回 {-1, 0, 0}。
 * 做法：交的端点必在 8 个候选点里（4 个原端点 + 4 个两两 LCA），
 * 筛出同时在两条路径上的点，取距离最远的一对。
 * 自带一份倍增 LCA，复制走不用带别的文件。
 *
 * 例：
 *   auto [len, x, y] = path_intersection(lca, a, b, c, d); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LCA {
    int n, LOG;
    vector<vector<int>> up;
    vector<int> dep;

    LCA(const vector<vector<int>> &g, int root = 1)
        : n((int)g.size() - 1), LOG(n > 0 ? 32 - __builtin_clz(n) : 1),
          up(LOG, vector<int>(n + 1)), dep(n + 1) {
        auto dfs = [&](auto &&self, int u, int p) -> void {
            up[0][u] = p;
            for (int v : g[u])
                if (v != p) {
                    dep[v] = dep[u] + 1;
                    self(self, v, u);
                }
        };
        dfs(dfs, root, 0);
        for (int j = 1; j < LOG; j++)
            for (int u = 1; u <= n; u++)
                up[j][u] = up[j - 1][up[j - 1][u]];
    }

    int lca(int a, int b) const {
        if (dep[a] < dep[b])
            swap(a, b);
        int d = dep[a] - dep[b];
        for (int j = 0; d; j++, d >>= 1)
            if (d & 1)
                a = up[j][a];
        if (a == b)
            return a;
        for (int j = LOG - 1; j >= 0; j--)
            if (up[j][a] != up[j][b])
                a = up[j][a], b = up[j][b];
        return up[0][a];
    }

    int dist(int a, int b) const { return dep[a] + dep[b] - 2 * dep[lca(a, b)]; }
};

inline tuple<int, int, int> path_intersection(const LCA &lca, int a, int b, int c,
                                              int d) {
    auto on_path = [&](int x, int u, int v) {
        return lca.dist(u, x) + lca.dist(x, v) == lca.dist(u, v);
    };
    int cand[8] = {
        a, b, c, d, lca.lca(a, c), lca.lca(a, d), lca.lca(b, c), lca.lca(b, d)};
    vector<int> hit;
    for (int x : cand) {
        if (!on_path(x, a, b) || !on_path(x, c, d))
            continue;
        if (find(hit.begin(), hit.end(), x) != hit.end())
            continue;
        hit.push_back(x);
    }
    if (hit.empty())
        return {-1, 0, 0};

    int best = -1, x = hit[0], y = hit[0];
    for (int i = 0; i < (int)hit.size(); i++)
        for (int j = i; j < (int)hit.size(); j++) {
            int d2 = lca.dist(hit[i], hit[j]);
            if (d2 > best) {
                best = d2;
                x = hit[i];
                y = hit[j];
            }
        }
    return {best, x, y};
}
