/**
 * @brief Prim 最小生成树
 * @complexity O(n^2)
 * @index 矩阵 0-based
 * @idea 每次选离生成树最近的点，再用它更新其他点到树的最便宜边。
 * @usage auto cost = prim(w); w[u][v] 是无向边权，重边取最小，无边填 PRIM_INF。
 * @pitfall 稠密图用矩阵版。不连通返回 nullopt。边权可为负，须小于 PRIM_INF。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline constexpr ll PRIM_INF = 1LL << 62;

inline optional<ll> prim(const vector<vector<ll>> &w) {
    int n = (int)w.size();
    if (n == 0)
        return 0;

    vector<ll> best(n, PRIM_INF);
    vector<bool> used(n);
    best[0] = 0;
    ll answer = 0;
    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int v = 0; v < n; v++)
            if (!used[v] && (u == -1 || best[v] < best[u]))
                u = v;
        if (best[u] == PRIM_INF)
            return nullopt;
        used[u] = true;
        answer += best[u];
        for (int v = 0; v < n; v++)
            if (!used[v] && w[u][v] < best[v])
                best[v] = w[u][v];
    }
    return answer;
}
