/**
 * @brief 线性递推第 n 项（Kitamasa）
 * @complexity O(k^2 log n)
 * @index 首项是 a[0]
 * @idea 在特征多项式的商环中算 x^n。其余式的系数就是前 k 项的线性组合系数。
 * @usage linear_recurrence(initial,c,n,mod) 求第 n 项，a[i]=c[0]*a[i-1]+...+c[k-1]*a[i-k]。
 * @pitfall initial 和 c 同长且非空，系数先归约。mod 为正 int，可为合数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline int linear_recurrence(const vector<int> &initial, const vector<int> &c,
                             ull n, int mod) {
    int k = (int)c.size();
    if (n < (ull)k)
        return initial[n];
    auto combine = [&](const vector<int> &a, const vector<int> &b) {
        vector<int> product(2 * k - 1);
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                product[i + j] = (product[i + j] + 1LL * a[i] * b[j]) % mod;
        for (int i = 2 * k - 2; i >= k; i--)
            for (int j = 0; j < k; j++)
                product[i - 1 - j] =
                    (product[i - 1 - j] + 1LL * product[i] * c[j]) % mod;
        product.resize(k);
        return product;
    };
    vector<int> result(k), base(k);
    result[0] = 1 % mod;
    if (k == 1)
        base[0] = c[0];
    else
        base[1] = 1 % mod;
    for (; n; n >>= 1, base = combine(base, base))
        if (n & 1)
            result = combine(result, base);

    ll answer = 0;
    for (int i = 0; i < k; i++)
        answer = (answer + 1LL * result[i] * initial[i]) % mod;
    return (int)answer;
}
