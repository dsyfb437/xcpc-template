/**
 * @brief 二次剩余（Tonelli–Shanks）
 * @complexity O(log p + s^2) 次模乘，另计找非剩余
 * @index 余数 0 到 p - 1
 * @idea 先用 Euler 判别。将 p - 1 拆成奇数乘 2^s，不断缩短待消去的 2 幂阶。
 * @usage sqrt_mod_prime(a, p) 返回最小平方根，无解返回 -1。另一个根是 p - root。
 * @pitfall p 必须为素数。a 可为负，根为 0 时只有一个根。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int sqrt_mod_prime(int a, int p) {
    a %= p;
    if (a < 0)
        a += p;
    if (a == 0 || p == 2)
        return a;
    auto power = [p](ll x, int e) {
        ll result = 1;
        for (; e; e >>= 1, x = x * x % p)
            if (e & 1)
                result = result * x % p;
        return result;
    };
    if (power(a, (p - 1) / 2) != 1)
        return -1;
    int q = p - 1, s = 0;
    while (q % 2 == 0) {
        q /= 2;
        s++;
    }
    int z = 2;
    while (power(z, (p - 1) / 2) == 1)
        z++;
    ll c = power(z, q), t = power(a, q), x = power(a, (q + 1) / 2);
    while (t != 1) {
        int i = 0;
        for (ll v = t; v != 1; v = v * v % p)
            i++;
        ll b = c;
        for (int j = 0; j < s - i - 1; j++)
            b = b * b % p;
        x = x * b % p;
        c = b * b % p;
        t = t * c % p;
        s = i;
    }
    return (int)min(x, p - x);
}
