/**
 * @brief 卡特兰数
 * @complexity O(n)
 * @index 下标 0 到 n
 * @idea 用 C[i] = C[i-1] * (4i - 2) / (i + 1) 递推，除法用线性递推的逆元。
 * @usage auto c = catalan_table(n, mod); c[i] 是第 i 个卡特兰数模 mod。
 * @pitfall mod 为素数且 n + 1 < mod。C[0] = 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> catalan_table(int n, int mod) {
    vector<int> c(n + 1), inv(n + 2);
    c[0] = inv[1] = 1;
    for (int i = 2; i <= n + 1; i++)
        inv[i] = mod - 1LL * (mod / i) * inv[mod % i] % mod;
    for (int i = 1; i <= n; i++) {
        ll ratio = (4LL * i - 2) % mod * inv[i + 1] % mod;
        c[i] = c[i - 1] * ratio % mod;
    }
    return c;
}
