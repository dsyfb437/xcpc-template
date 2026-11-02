/**
 * @brief 重链剖分
 * @complexity 预处理 O(n)，路径拆成 O(log n) 段
 * @index 点 1-based，pos 0-based
 *
 * @idea 把树上路径拆成 O(log n) 段连续下标，接线段树/树状数组做路径查询、修改。
 * @usage `HLD hld(g, root);` 之后 `hld.path(u, v, [&](int l, int r) { ... });`
 *        每段是 pos 的闭区间 [l, r]（0-based）；子树 u 是 [pos[u], pos[u] + sz[u] - 1]。
 * 
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct HLD {
    int n, cur = 0;
    const vector<vector<int>> &g;
    vector<int> par, dep, sz, heavy, top, pos;

    HLD(const vector<vector<int>> &g, int root = 1)
        : n((int)g.size() - 1), g(g), par(n + 1), dep(n + 1), sz(n + 1),
          heavy(n + 1, 0), top(n + 1), pos(n + 1) {
        dfs1(root, 0);
        dfs2(root, root);
    }

    void dfs1(int u, int p) {
        par[u] = p;
        sz[u] = 1;
        for (int v : g[u])
            if (v != p) {
                dep[v] = dep[u] + 1;
                dfs1(v, u);
                sz[u] += sz[v];
                if (sz[v] > sz[heavy[u]])
                    heavy[u] = v;
            }
    }

    void dfs2(int u, int h) {
        top[u] = h;
        pos[u] = cur++;
        if (heavy[u])
            dfs2(heavy[u], h);
        for (int v : g[u])
            if (v != par[u] && v != heavy[u])
                dfs2(v, v);
    }

    // 把 u-v 路径拆成若干闭区间 [l, r]，依次调用 f(l, r)。
    template <class F> void path(int u, int v, F f) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]])
                swap(u, v);
            f(pos[top[u]], pos[u]);
            u = par[top[u]];
        }
        if (dep[u] > dep[v])
            swap(u, v);
        f(pos[u], pos[v]);
    }

    int lca(int u, int v) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]])
                swap(u, v);
            u = par[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }
};
