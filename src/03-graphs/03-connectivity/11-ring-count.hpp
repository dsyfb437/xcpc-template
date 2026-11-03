/**
 * @brief 无向简单环计数
 * @complexity O(n^2 2^n)
 * @index 点 0-based
 * @idea 固定环中编号最小的点 s。状压 DP 数从 s 到每个末点的简单路径，能连回 s 就闭环。
 * @usage count_simple_cycles(adj) 返回所有长度至少 3 的环数，adj 是 0/1 邻接矩阵。
 * @pitfall 简单无向图，正反方向只计一次。建议 n <= 18，空间 O(n 2^n)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline ull count_simple_cycles(const vector<vector<int>> &adj) {
    int n = (int)adj.size();
    ull total = 0;
    for (int s = 0; s + 2 < n; s++) {
        int k = n - s - 1, states = 1 << k;
        vector dp(states, vector<ull>(k));
        for (int v = 0; v < k; v++)
            if (adj[s][s + 1 + v])
                dp[1 << v][v] = 1;
        for (int mask = 1; mask < states; mask++) {
            for (int u = 0; u < k; u++) {
                auto ways = dp[mask][u];
                if (!ways)
                    continue;
                if (popcount((unsigned)mask) >= 2 && adj[s + 1 + u][s])
                    total += ways;
                for (int v = 0; v < k; v++)
                    if (!(mask >> v & 1) && adj[s + 1 + u][s + 1 + v])
                        dp[mask | (1 << v)][v] += ways;
            }
        }
    }
    return total / 2;
}
