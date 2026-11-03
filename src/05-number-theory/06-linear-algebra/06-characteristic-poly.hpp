/**
 * @brief 特征多项式（Hessenberg 消元）
 * @complexity O(n^3 + n log mod)
 * @index 系数按低次到高次
 * @idea 用相似变换消掉次对角线下方的元素。此时 det(xI-A) 可以按末列展开递推。
 * @usage characteristic_polynomial(a, mod) 返回 det(xI-A) 的系数，a 可含负数。
 * @pitfall a 是方阵，mod 是 int 范围的素数。空矩阵返回 {1}。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> characteristic_polynomial(vector<vector<ll>> a, int mod) {
    int n = (int)a.size();
    auto norm = [mod](ll x) { return (x % mod + mod) % mod; };
    auto inverse = [mod](ll x) {
        ll result = 1;
        for (int e = mod - 2; e; e >>= 1, x = x * x % mod)
            if (e & 1)
                result = result * x % mod;
        return result;
    };
    for (auto &row : a)
        for (ll &x : row)
            x = norm(x);
    for (int i = 1; i < n; i++) {
        int pivot = i;
        while (pivot < n && !a[pivot][i - 1])
            pivot++;
        if (pivot == n)
            continue;
        swap(a[i], a[pivot]);
        for (int row = 0; row < n; row++)
            swap(a[row][i], a[row][pivot]);

        ll inv = inverse(a[i][i - 1]);
        for (int j = i + 1; j < n; j++) {
            ll factor = a[j][i - 1] * inv % mod;
            if (!factor)
                continue;
            for (int k = 0; k < n; k++)
                a[j][k] = norm(a[j][k] - factor * a[i][k]);
            for (int k = 0; k < n; k++)
                a[k][i] = norm(a[k][i] + factor * a[k][j]);
        }
    }

    vector<vector<int>> poly(n + 1);
    poly[0] = {1};
    for (int i = 1; i <= n; i++) {
        poly[i].resize(i + 1);
        for (int k = 0; k < i; k++) {
            poly[i][k + 1] = poly[i - 1][k];
        }
        for (int k = 0; k < i; k++)
            poly[i][k] = norm(poly[i][k] - a[i - 1][i - 1] * poly[i - 1][k]);
        ll product = 1;
        for (int j = i - 2; j >= 0; j--) {
            product = product * a[j + 1][j] % mod;
            ll factor = product * a[j][i - 1] % mod;
            for (int k = 0; k <= j; k++)
                poly[i][k] = norm(poly[i][k] - factor * poly[j][k]);
        }
    }
    return poly.back();
}
