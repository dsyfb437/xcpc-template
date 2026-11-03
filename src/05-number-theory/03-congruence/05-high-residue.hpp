/**
 * @brief 高次剩余（素数模开 k 次根）
 * @complexity 期望 O(sqrt(p) + r log r)，另计找原根
 * @index 余数 0 到 p - 1
 * @idea 把非零余数写成原根的幂。用 BSGS 求目标指数，再解关于指数的一元线性同余。
 * @usage roots_mod_prime(a, k, p) 返回 x^k = a mod p 的全部根，升序去重。
 * @pitfall p 为素数，k 为正 int，r 是答案数。a 可为负，答案可能多达 p - 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int high_residue_generator(int p) {
    if (p == 2)
        return 1;
    vector<int> divisors;
    int rest = p - 1;
    for (int q = 2; q <= rest / q; q++) {
        if (rest % q)
            continue;
        divisors.push_back(q);
        do {
            rest /= q;
        } while (rest % q == 0);
    }
    if (rest > 1)
        divisors.push_back(rest);

    auto power = [p](ll a, int e) {
        ll result = 1;
        for (; e; e >>= 1, a = a * a % p)
            if (e & 1)
                result = result * a % p;
        return result;
    };
    for (int g = 2; g < p; g++) {
        bool ok = true;
        for (int q : divisors)
            if (power(g, (p - 1) / q) == 1) {
                ok = false;
                break;
            }
        if (ok)
            return g;
    }
    return -1;
}

inline vector<int> roots_mod_prime(int a, int k, int p) {
    a %= p;
    if (a < 0)
        a += p;
    if (a == 0 || p == 2)
        return {a};

    int g = high_residue_generator(p), order = p - 1;
    auto power = [p, g](int e) {
        ll result = 1, base = g;
        for (; e; e >>= 1, base = base * base % p)
            if (e & 1)
                result = result * base % p;
        return result;
    };
    int step = (int)sqrt(order) + 1;
    unordered_map<int, int> baby;
    baby.reserve(step);
    for (int j = 0, value = 1; j < step; j++) {
        baby.try_emplace(value, j);
        value = 1LL * value * g % p;
    }
    ll backward = power((order - step % order) % order);
    int exponent = -1;
    for (int i = 0; i <= step; i++) {
        auto it = baby.find(a);
        if (it != baby.end()) {
            exponent = (1LL * i * step + it->second) % order;
            break;
        }
        a = a * backward % p;
    }
    int d = gcd(k, order);
    if (exponent < 0 || exponent % d)
        return {};

    ll u = k / d, period = order / d, v = period, x = 1, y = 0;
    while (v) {
        ll q = u / v, r = u % v;
        u = v;
        v = r;
        ll z = x - q * y;
        x = y;
        y = z;
    }
    int start = ((x % period + period) % period * (exponent / d)) % period;
    ll value = power(start), ratio = power(period);
    vector<int> roots;
    for (int i = 0; i < d; i++) {
        roots.push_back(value);
        value = value * ratio % p;
    }
    sort(roots.begin(), roots.end());
    return roots;
}
