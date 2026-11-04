/**
 * @brief 矩阵快速幂
 * @complexity O(n^3 log k)
 * @index vector 0-based
 * @pitfall 两个矩阵须为同阶方阵，k 非负、p 为正。元素可为负，乘法用 __int128。
 *
 * 方阵乘法取模，求 A^k mod p。n 一般不超过 100，k 可以到 1e18。
 * 例：
 *   Matrix a(n, vector<ll>(n));
 *   Matrix r = mat_pow(a, k, p);
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Matrix = vector<vector<ll>>;

inline Matrix mat_mul(const Matrix &a, const Matrix &b, ll p) {
    int n = a.size();
    Matrix c(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            if (a[i][k] == 0)
                continue;
            for (int j = 0; j < n; j++) {
                c[i][j] = (ll)((c[i][j] + (__int128)a[i][k] * b[k][j]) % p);
                if (c[i][j] < 0)
                    c[i][j] += p;
            }
        }
    return c;
}

inline Matrix mat_pow(Matrix a, ll k, ll p) {
    int n = a.size();
    Matrix r(n, vector<ll>(n, 0));
    for (int i = 0; i < n; i++)
        r[i][i] = 1 % p;
    while (k) {
        if (k & 1)
            r = mat_mul(r, a, p);
        a = mat_mul(a, a, p);
        k >>= 1;
    }
    return r;
}
