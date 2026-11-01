/**
 * @brief 组合数（模素数）
 * @complexity 预处理 O(n + log mod)，单次 O(1)
 *
 * mod 必须是素数，且所有 n < mod。
 * 做法：先算阶乘 fac，用费马小定理求 ifac[n]，再倒推 ifac[i-1] = ifac[i] * i。
 * C(n, k) 在 k < 0 或 k > n 时返回 0。
 * 例：
 *   Comb comb(200000, 1000000007);
 *   ll c = comb.C(n, k);
 */
#pragma once
#include <bits/stdc++.h>
#include "mod_pow.hpp"
using namespace std;
using ll = long long;

struct Comb {
    ll mod;
    vector<ll> fac, ifac;

    Comb(int n, ll mod) : mod(mod), fac(n + 1), ifac(n + 1) {
        fac[0] = 1;
        for (int i = 1; i <= n; i++)
            fac[i] = fac[i - 1] * i % mod;
        ifac[n] = mod_pow(fac[n], mod - 2, mod);
        for (int i = n; i >= 1; i--)
            ifac[i - 1] = ifac[i] * i % mod;
    }

    ll C(int n, int k) const {
        if (k < 0 || k > n)
            return 0;
        return fac[n] * ifac[k] % mod * ifac[n - k] % mod;
    }

    ll P(int n, int k) const {
        if (k < 0 || k > n)
            return 0;
        return fac[n] * ifac[n - k] % mod;
    }
};
