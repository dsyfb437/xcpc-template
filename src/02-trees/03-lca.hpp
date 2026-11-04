/**
 * @brief 最近公共祖先（倍增）
 * @complexity 预处理 O(n log n)，单次 O(log n)
 * @index 点 1-based
 *
 * @idea 求树上两点的 LCA 和距离。
 * @usage `LCA lca(g, root);` 之后 `lca.lca(a, b)`、`lca.dist(a, b)`。
 * @pitfall g 是非空无权树。带边权时另存根到每个点的距离。
 *
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct LCA {
    int n, LOG;
    vector<vector<int>> up;
    vector<int> dep;

    LCA(const vector<vector<int>> &g, int root = 1)
        : n((int)g.size() - 1), LOG(n > 0 ? 32 - __builtin_clz(n) : 1),
          up(LOG, vector<int>(n + 1)), dep(n + 1) {
        vector<int> order{root};
        for (int i = 0; i < n; i++) {
            int u = order[i];
            for (int v : g[u]) {
                if (v == up[0][u]) continue;
                up[0][v] = u;
                dep[v] = dep[u] + 1;
                order.push_back(v);
            }
        }
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
