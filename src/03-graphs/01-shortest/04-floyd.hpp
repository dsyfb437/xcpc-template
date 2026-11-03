/**
 * @brief Floyd 全源最短路
 * @complexity O(n^3)
 * @index 矩阵 0-based
 * @idea 枚举允许作为中转的点 k，用 i 到 k 再到 j 尝试更新 d[i][j]。
 * @usage `floyd(d);` 对角线先设为 0，重边取最小值，无边填 FLOYD_INF。
 * @pitfall 有负环时 d[i][i] < 0 可用于判环。负环上的最短距离没有有限值。有限加法需放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline constexpr ll FLOYD_INF = 1LL << 62;

inline void floyd(vector<vector<ll>> &d) {
    int n = (int)d.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++) {
            if (d[i][k] == FLOYD_INF)
                continue;
            for (int j = 0; j < n; j++)
                if (d[k][j] != FLOYD_INF)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        }
}
