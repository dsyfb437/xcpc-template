/**
 * @brief Powerful Number 筛
 * @complexity 枚举 O(sqrt(n)) 个强力数，另计回调
 * @index 统计 1 到 n
 * @idea 写成 f = g * h，且 h(p) = 0。只枚举每个质因子指数至少为 2 的 d，累加 h(d) G(n/d)。
 * @usage powerful_sum(n, mod, prefix_g, h) 求 f 的前缀和。h(p, e) 返回 h(p^e)，prefix_g(x) 返回 g 的前缀和。
 * @pitfall f、g、h 为积性函数且在 1 处取 1。n 在 0 到 10^12，mod 为正 int，两个回调的返回值可为负。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

template <class Prefix, class PrimePower>
int powerful_sum(ll n, int mod, Prefix prefix_g, PrimePower h) {
    if (!n)
        return 0;
    int root = (int)sqrt((ld)n);
    while (root + 1 <= n / (root + 1))
        root++;
    while (root > n / root)
        root--;

    vector<bool> composite(root + 1);
    vector<int> primes;
    for (int p = 2; p <= root; p++)
        if (!composite[p]) {
            primes.push_back(p);
            if (p <= root / p)
                for (int j = p * p; j <= root; j += p)
                    composite[j] = true;
        }

    auto norm = [mod](ll x) { return (x % mod + mod) % mod; };
    ll answer = 0;
    auto dfs = [&](auto &&self, ll limit, int start, ll weight) -> void {
        answer = (answer + weight * norm(prefix_g(limit))) % mod;
        for (int i = start; i < (int)primes.size(); i++) {
            ll p = primes[i];
            if (p > limit / p)
                break;
            ll power = p * p;
            for (int e = 2; power <= limit; e++) {
                ll next = weight * norm(h(p, e)) % mod;
                if (next)
                    self(self, limit / power, i + 1, next);
                if (power > limit / p)
                    break;
                power *= p;
            }
        }
    };
    dfs(dfs, n, 0, 1 % mod);
    return (int)answer;
}
