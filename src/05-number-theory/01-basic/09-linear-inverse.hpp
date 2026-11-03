/**
 * @brief 线性递推逆元
 * @complexity O(n)
 * @index 下标 1 到 n
 * @idea 把 p 写成 q i + r，模 p 下有 inv[i] = -q inv[r]，按 i 从小到大填表。
 * @usage auto inv = linear_inverses(n, p); inv[i] 是 i 在模 p 下的逆元。
 * @pitfall p 为素数，0 <= n < p，inv[0] 没有意义。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> linear_inverses(int n, int p) {
    vector<int> inv(n + 1);
    if (n >= 1)
        inv[1] = 1;
    for (int i = 2; i <= n; i++)
        inv[i] = p - (1LL * (p / i) * inv[p % i] % p);
    return inv;
}
