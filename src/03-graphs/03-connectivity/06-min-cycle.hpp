/**
 * @brief 无向图最小环
 * @complexity O(n^3)
 * @index 点 0-based
 * @idea Floyd 枚举环中编号最大的点 k。放开 k 之前，用 i 到 j 的最短路和 i、j 各自到 k 的边拼环。
 * @usage minimum_cycle(w) 返回最小环长度，w 是邻接矩阵，无边填 MIN_CYCLE_INF。无环返回 -1。
 * @pitfall 非负权简单无向图，只考虑至少三条边的环。有限环长须小于 MIN_CYCLE_INF。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline constexpr ll MIN_CYCLE_INF = 1LL << 60;

inline ll minimum_cycle(const vector<vector<ll>> &w) {
    int n = (int)w.size();
    auto d = w;
    for (int i = 0; i < n; i++)
        d[i][i] = 0;

    ll answer = MIN_CYCLE_INF;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < k; i++)
            for (int j = i + 1; j < k; j++)
                answer = min(answer, d[i][j] + w[i][k] + w[j][k]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    }
    return answer == MIN_CYCLE_INF ? -1 : answer;
}
