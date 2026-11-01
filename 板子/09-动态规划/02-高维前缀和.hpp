/**
 * @brief 高维前缀和（子集和）
 * @complexity O(n 2^n)
 * @index mask 0-based
 *
 * f[s] 变成 sum_{t ⊆ s} f[t]；求超集和把 if 条件反过来。
 * 逆变换是差分，两个函数配套用。
 *
 * 例：
 *   subset_zeta(f, n);    // f[s] 变成 sum_{t ⊆ s} f[t]
 *   subset_mobius(f, n);  // 还原 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void subset_zeta(vector<ll> &f, int n) {
    for (int i = 0; i < n; i++)
        for (int s = 0; s < (1 << n); s++)
            if (s >> i & 1)
                f[s] += f[s ^ (1 << i)];
}

void subset_mobius(vector<ll> &f, int n) {
    for (int i = 0; i < n; i++)
        for (int s = 0; s < (1 << n); s++)
            if (s >> i & 1)
                f[s] -= f[s ^ (1 << i)];
}
