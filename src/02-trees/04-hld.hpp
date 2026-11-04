/**
 * @brief 重链剖分
 * @complexity 预处理 O(n)，路径拆成 O(log n) 段
 * @index 点 1-based，pos 0-based
 *
 * @idea 把树上路径拆成 O(log n) 段连续下标，接线段树/树状数组做路径查询、修改。
 * @usage `HLD hld(g, root);` 之后 `hld.path(u, v, [&](int l, int r) { ... });`
 *        每段是 pos 的闭区间 [l, r]（0-based）；子树 u 是 [pos[u], pos[u] + sz[u] - 1]。
 * @pitfall g 是非空树，且在 hld 使用期间有效。path 回调的段没有按 u 到 v 的方向排序，适合顺序无关的统计。
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

    void dfs1(int root, int parent) {
        vector<int> order{root};
        par[root] = parent;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            sz[u] = 1;
            heavy[u] = 0;
            for (int v : g[u]) {
                if (v == par[u]) continue;
                par[v] = u;
                dep[v] = dep[u] + 1;
                order.push_back(v);
            }
        }
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int u = order[i], p = par[u];
            sz[p] += sz[u];
            if (sz[u] >= sz[heavy[p]]) heavy[p] = u;
        }
    }

    void dfs2(int root, int head) {
        vector<pair<int, int>> stack{{root, head}};
        while (!stack.empty()) {
            auto [u, h] = stack.back();
            stack.pop_back();
            top[u] = h;
            pos[u] = cur++;
            for (auto it = g[u].rbegin(); it != g[u].rend(); ++it) {
                int v = *it;
                if (v != par[u] && v != heavy[u]) stack.emplace_back(v, v);
            }
            if (heavy[u]) stack.emplace_back(heavy[u], h);
        }
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
