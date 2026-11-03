/**
 * @brief 矩阵树定理（无向图）
 * @complexity O(n^3 + n log mod)
 * @index 点 0-based
 * @idea 对每条边给拉普拉斯矩阵的两端对角线加一，两个非对角项减一。删同一行列后求行列式。
 * @usage count_spanning_trees(n, edges, mod) 返回生成树数模 mod。
 * @pitfall mod 是 int 范围的素数，重边逐条传入，自环忽略。单点图答案为 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int matrix_tree_det(vector<vector<ll>> a, int mod) {
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

inline int count_spanning_trees(int n, const vector<pair<int, int>> &edges,
                                int mod) {
    if (n == 0)
        return 0;
    vector a(n - 1, vector<ll>(n - 1));
    for (auto [u, v] : edges) {
        if (u == v)
            continue;
        if (u != n - 1)
            a[u][u]++;
        if (v != n - 1)
            a[v][v]++;
        if (u != n - 1 && v != n - 1) {
            a[u][v]--;
            a[v][u]--;
        }
    }
    return matrix_tree_det(move(a), mod);
}
