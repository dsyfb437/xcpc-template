/**
 * @brief 树形 DP（带权最大独立集）
 * @complexity O(n)
 * @index 点 1-based
 * @idea 每点存选与不选两种值。选父亲时儿子都不能选，不选父亲时儿子各自取较优值。
 * @usage tree_independent_set(g,weight,root) 返回最大权值。
 * @pitfall g 为非空无向树，weight[0] 留空。允许负权与空集，返回 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline __int128 tree_independent_set(const vector<vector<int>> &g,
                                     const vector<ll> &weight, int root = 1) {
    vector<int> parent(g.size()), order{root};
    for (int i = 0; i < (int)order.size(); i++) {
        int u = order[i];
        for (int v : g[u])
            if (v != parent[u]) {
                parent[v] = u;
                order.push_back(v);
            }
    }
    vector<array<__int128, 2>> dp(g.size());
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int u = order[i], p = parent[u];
        dp[u][1] += weight[u];
        if (p) {
            dp[p][0] += max(dp[u][0], dp[u][1]);
            dp[p][1] += dp[u][0];
        }
    }
    return max(dp[root][0], dp[root][1]);
}
