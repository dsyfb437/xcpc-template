/**
 * @brief 树上莫队（路径不同值个数）
 * @complexity 预处理 O(n log n)，排序 O(q log q)，移动 O((n+q)√n)
 * @index 点从 0 编号，路径含两个端点
 * @idea DFS 进出各记一次。区间内出现奇数次的点就在路径上。若两端都不是 LCA，需额外计入 LCA。
 * @usage auto ans = mo_tree_path_distinct(g, values, paths);
 *   paths 每项是 {u,v}，询问 u 到 v 的路径。
 * @pitfall g 是一棵无向树，根取 0。LCA 额外加的是它的值，窗口已有同值时不能再计数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> mo_tree_path_distinct(const vector<vector<int>> &g,
                                         const vector<ll> &values,
                                         const vector<pair<int, int>> &paths) {
    int n = (int)g.size();
    if (n == 0)
        return {};
    int levels = bit_width((unsigned)n);
    vector<vector<int>> parent(levels, vector<int>(n));
    vector<int> depth(n), in(n), out(n), euler, stack = {0};
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        if (u < 0) {
            out[~u] = (int)euler.size();
            euler.push_back(~u);
            continue;
        }
        in[u] = (int)euler.size();
        euler.push_back(u);
        stack.push_back(~u);
        for (int v : g[u]) {
            if (v == parent[0][u])
                continue;
            parent[0][v] = u;
            depth[v] = depth[u] + 1;
            stack.push_back(v);
        }
    }
    for (int k = 1; k < levels; k++)
        for (int u = 0; u < n; u++)
            parent[k][u] = parent[k - 1][parent[k - 1][u]];
    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v])
            swap(u, v);
        int gap = depth[u] - depth[v];
        for (int k = 0; k < levels; k++)
            if ((gap >> k) & 1)
                u = parent[k][u];
        if (u == v)
            return u;
        for (int k = levels - 1; k >= 0; k--) {
            if (parent[k][u] == parent[k][v])
                continue;
            u = parent[k][u];
            v = parent[k][v];
        }
        return parent[0][u];
    };

    struct Query {
        int l, r, extra, id;
    };
    vector<Query> queries;
    for (auto [u, v] : paths) {
        if (in[u] > in[v])
            swap(u, v);
        int p = lca(u, v), id = (int)queries.size();
        if (p == u)
            queries.push_back({in[u], in[v] + 1, -1, id});
        else
            queries.push_back({out[u], in[v] + 1, p, id});
    }

    int block = max(1, (int)sqrt(euler.size()));
    sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
        int x = a.l / block, y = b.l / block;
        if (x != y)
            return x < y;
        return x % 2 == 0 ? a.r < b.r : a.r > b.r;
    });

    auto keys = values;
    sort(keys.begin(), keys.end());
    keys.erase(unique(keys.begin(), keys.end()), keys.end());
    vector<int> rank(n);
    for (int u = 0; u < n; u++)
        rank[u] = lower_bound(keys.begin(), keys.end(), values[u]) - keys.begin();
    vector<int> frequency(keys.size()), ans(paths.size());
    vector<bool> active(n);
    int l = 0, r = 0, distinct = 0;
    auto toggle = [&](int i) {
        int u = euler[i], x = rank[u];
        if (active[u])
            distinct -= --frequency[x] == 0;
        else
            distinct += frequency[x]++ == 0;
        active[u] = !active[u];
    };
    for (const auto &q : queries) {
        while (l > q.l)
            toggle(--l);
        while (r < q.r)
            toggle(r++);
        while (l < q.l)
            toggle(l++);
        while (r > q.r)
            toggle(--r);
        ans[q.id] = distinct;
        if (q.extra != -1 && frequency[rank[q.extra]] == 0)
            ans[q.id]++;
    }
    return ans;
}
