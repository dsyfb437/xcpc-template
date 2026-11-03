/**
 * @brief 行列式（模素数）
 * @complexity O(n^3 + n log mod)
 * @index 矩阵 0-based
 * @idea 高斯消元变成上三角，行列式等于对角线乘积。每交换两行就变号。
 * @usage determinant_mod(a, mod) 返回行列式模 mod，a 可以有负元素。
 * @pitfall a 是方阵，mod 是正 int 范围的素数。空矩阵的行列式为 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int determinant_mod(vector<vector<ll>> a, int mod) {
    int n = (int)a.size();
    auto power = [mod](ll x, int e) {
        ll result = 1;
        for (; e; e >>= 1, x = x * x % mod)
            if (e & 1)
                result = result * x % mod;
        return result;
    };
    for (auto &row : a)
        for (auto &value : row)
            value = (value % mod + mod) % mod;

    ll answer = 1;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && !a[pivot][i])
            pivot++;
        if (pivot == n)
            return 0;
        if (pivot != i) {
            swap(a[pivot], a[i]);
            answer = (mod - answer) % mod;
        }
        answer = answer * a[i][i] % mod;
        ll inv = power(a[i][i], mod - 2);
        for (int j = i + 1; j < n; j++) {
            ll factor = a[j][i] * inv % mod;
            for (int k = i + 1; k < n; k++)
                a[j][k] = (a[j][k] - factor * a[i][k] % mod + mod) % mod;
        }
    }
    return (int)answer;
}
