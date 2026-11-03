/**
 * @brief LGV 引理（不相交路径组计数）
 * @complexity O(k^3 + k log mod)，另计路径矩阵预处理
 * @index 起终点编号 0-based
 * @idea path[i][j] 是 DAG 中第 i 个起点到第 j 个终点的路径数。行列式中相交路径组会成对抵消。
 * @usage lgv_count(path, mod) 求点不相交路径组的带符号计数。
 * @pitfall mod 为 int 范围素数。若只能按同编号配对，行列式才直接等于该配对的无符号计数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int lgv_count(vector<vector<ll>> a, int mod) {
    int n = (int)a.size();
    auto power = [&](ll x, int k) {
        ll result = 1;
        for (; k; k >>= 1, x = x * x % mod)
            if (k & 1)
                result = result * x % mod;
        return result;
    };
    for (auto &row : a)
        for (ll &x : row)
            x = (x % mod + mod) % mod;

    ll det = 1;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && a[pivot][i] == 0)
            pivot++;
        if (pivot == n)
            return 0;
        if (pivot != i) {
            swap(a[pivot], a[i]);
            det = (mod - det) % mod;
        }
        det = det * a[i][i] % mod;
        ll inv = power(a[i][i], mod - 2);
        for (int j = i + 1; j < n; j++) {
            ll ratio = a[j][i] * inv % mod;
            for (int k = i + 1; k < n; k++)
                a[j][k] = (a[j][k] - ratio * a[i][k] % mod + mod) % mod;
        }
    }
    return (int)det;
}
