/**
 * @brief 杜教筛（phi 与 mu 前缀和）
 * @complexity 预处理 O(B)，单次期望 O(n / sqrt(B))
 * @index 统计 1 到 n
 * @idea 利用 phi * 1 = id、mu * 1 = epsilon。把卷积前缀的 d = 1 项移到等号左边，其余按 n / d 分块并记忆化。
 * @usage DuSieve sieve(B); sieve.phi(n) 与 sieve.mu(n) 分别查前缀和。
 * @pitfall n 非负，B >= 1。单次查询常取 B 约为 n^(2/3)，phi 用 __int128 返回。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DuSieve {
    vector<ll> phi_prefix, mu_prefix;
    unordered_map<ll, __int128> phi_cache;
    unordered_map<ll, ll> mu_cache;

    explicit DuSieve(int B) : phi_prefix(B + 1), mu_prefix(B + 1) {
        vector<int> primes, least(B + 1);
        phi_prefix[1] = mu_prefix[1] = 1;
        for (int i = 2; i <= B; i++) {
            if (!least[i]) {
                least[i] = i;
                primes.push_back(i);
                phi_prefix[i] = i - 1;
                mu_prefix[i] = -1;
            }
            for (int p : primes) {
                if (p > B / i)
                    break;
                least[i * p] = p;
                if (i % p == 0) {
                    phi_prefix[i * p] = phi_prefix[i] * p;
                    break;
                }
                phi_prefix[i * p] = phi_prefix[i] * (p - 1);
                mu_prefix[i * p] = -mu_prefix[i];
            }
        }
        for (int i = 1; i <= B; i++) {
            phi_prefix[i] += phi_prefix[i - 1];
            mu_prefix[i] += mu_prefix[i - 1];
        }
    }

    __int128 phi(ll n) {
        if (n < (ll)phi_prefix.size())
            return phi_prefix[n];
        auto it = phi_cache.find(n);
        if (it != phi_cache.end())
            return it->second;
        __int128 result = (__int128)n * (n + (__int128)1) / 2;
        for (ll l = 2; l <= n;) {
            ll q = n / l, r = n / q;
            result -= (r - l + 1) * phi(q);
            if (r == n)
                break;
            l = r + 1;
        }
        return phi_cache.emplace(n, result).first->second;
    }

    ll mu(ll n) {
        if (n < (ll)mu_prefix.size())
            return mu_prefix[n];
        auto it = mu_cache.find(n);
        if (it != mu_cache.end())
            return it->second;
        __int128 result = 1;
        for (ll l = 2; l <= n;) {
            ll q = n / l, r = n / q;
            result -= (__int128)(r - l + 1) * mu(q);
            if (r == n)
                break;
            l = r + 1;
        }
        return mu_cache.emplace(n, (ll)result).first->second;
    }
};
