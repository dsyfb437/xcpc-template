/**
 * @brief 范德蒙德卷积
 * @complexity O(min(k, n + m - k) + log mod)
 * @index 非负 n、m、k
 * @idea 从两堆共 n + m 个元素里选 k 个。按第一堆选了多少分类，卷积和就等于 C(n + m, k)。
 * @usage vandermonde(n, m, k, mod) 求 sum_i C(n,i) C(m,k-i) 模 mod。
 * @pitfall mod 是素数且 n + m < mod。k 超过总数时返回 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int vandermonde(int n, int m, int k, int mod) {
    int total = n + m;
    if (k < 0 || k > total)
        return 0;
    k = min(k, total - k);
    ll top = 1, bottom = 1;
    for (int i = 1; i <= k; i++) {
        top = top * (total - i + 1) % mod;
        bottom = bottom * i % mod;
    }
    for (int e = mod - 2; e; e >>= 1, bottom = bottom * bottom % mod)
        if (e & 1)
            top = top * bottom % mod;
    return (int)top;
}
