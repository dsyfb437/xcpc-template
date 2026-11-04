/**
 * @brief Floyd 全源最短路
 * @complexity O(n^3)
 * @index 矩阵 0-based
 * @idea 枚举允许作为中转的点 k，用 i 到 k 再到 j 尝试更新 d[i][j]。
 * @usage `floyd(d);` 对角线先设为 0，重边取最小值，无边填 FLOYD_INF。
 * @pitfall 有限路径长度须严格在 ±FLOYD_INF 内。有负环时 d[i][i] < 0 可用于判环，受负环影响的距离没有有限值。
 *   负环可能使更新值反复变小，低于 -FLOYD_INF 时截到该值。
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
                if (d[k][j] != FLOYD_INF) {
                    __int128 next = (__int128)d[i][k] + d[k][j];
                    next = max(next, -(__int128)FLOYD_INF);
                    if (next < d[i][j]) d[i][j] = (ll)next;
                }
        }
}
