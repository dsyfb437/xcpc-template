/**
 * @brief 哈密顿路径与回路
 * @complexity O(n 2^n)
 * @index 点 0-based
 * @idea ends[mask] 用位集记录覆盖 mask 后所有可能的末点。能从上一状态的末点走到 u，就把 u 加进当前状态。
 * @usage hamilton_path(adj) 返回一条路径。hamilton_path(adj, true) 固定从 0 出发找回路，返回顺序不重复写 0。
 * @pitfall adj[u][v] 为 0 或 1，支持有向图。建议 n <= 22。无解返回空数组，回路至少经过三个点。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> hamilton_path(const vector<vector<int>> &adj,
                                 bool cycle = false) {
    int n = (int)adj.size();
    if (n == 0 || (cycle && n < 3))
        return {};
    vector<unsigned> incoming(n);
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++)
            if (adj[u][v])
                incoming[v] |= 1U << u;

    unsigned full = (1U << n) - 1;
    vector<unsigned> ends(full + 1);
    for (unsigned mask = 1; mask <= full; mask++) {
        if (cycle && !(mask & 1))
            continue;
        if (has_single_bit(mask)) {
            ends[mask] = mask;
            continue;
        }
        for (unsigned bits = mask; bits; bits &= bits - 1) {
            int v = countr_zero(bits);
            unsigned bit = 1U << v;
            if (ends[mask ^ bit] & incoming[v])
                ends[mask] |= bit;
        }
    }
    unsigned last = ends[full];
    if (cycle)
        last &= incoming[0];
    if (!last)
        return {};

    vector<int> path;
    for (unsigned mask = full; mask;) {
        int v = countr_zero(last);
        path.push_back(v);
        mask ^= 1U << v;
        last = ends[mask] & incoming[v];
    }
    reverse(path.begin(), path.end());
    return path;
}
