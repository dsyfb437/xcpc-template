/**
 * @brief 伯努利数
 * @complexity O(n^2)
 * @index 下标 0 到 n
 * @idea 用 Akiyama–Tanigawa 三角递推，维护当前行，再把 B[1] 改成常用的 -1/2 约定。
 * @usage bernoulli_table(n, mod) 返回 B[0] 到 B[n] 在模 mod 下的值。
 * @pitfall mod 为素数且 n + 1 < mod，保证所有分母可逆。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> bernoulli_table(int n, int mod) {
    vector<int> b(n + 1), a(n + 1), inv(n + 2);
    inv[1] = 1;
    for (int i = 2; i <= n + 1; i++)
        inv[i] = mod - 1LL * (mod / i) * inv[mod % i] % mod;

    for (int i = 0; i <= n; i++) {
        a[i] = inv[i + 1];
        for (int j = i; j >= 1; j--) {
            ll difference = a[j - 1] - a[j] + (ll)mod;
            a[j - 1] = difference * j % mod;
        }
        b[i] = a[0];
    }
    if (n >= 1)
        b[1] = mod - b[1];
    return b;
}
