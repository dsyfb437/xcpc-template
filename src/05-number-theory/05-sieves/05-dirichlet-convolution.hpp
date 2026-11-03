/**
 * @brief 狄利克雷卷积
 * @complexity O(n log n)
 * @index 下标 1 到 n
 * @idea 枚举因子对 d、k，将 f[d] * g[k] 加到 h[d*k]。
 * @usage dirichlet_convolution(f, g, mod) 返回卷积表，输入数组同长。
 * @pitfall mod 为正 int，可为合数。输入允许负值，结果归约到 [0, mod)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> dirichlet_convolution(const vector<int> &f, const vector<int> &g,
                                         int mod) {
    int n = (int)f.size() - 1;
    vector<int> h(n + 1);
    for (int d = 1; d <= n; d++)
        for (int k = 1; k <= n / d; k++) {
            int &value = h[d * k];
            value = (value + 1LL * f[d] * g[k]) % mod;
            if (value < 0)
                value += mod;
        }
    return h;
}
