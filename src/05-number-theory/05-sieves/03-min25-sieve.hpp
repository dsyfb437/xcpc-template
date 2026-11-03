/**
 * @brief Min_25 筛（积性函数前缀和）
 * @complexity 素数筛 O(n^(3/4))，另计质因子递归
 * @index 统计 1 到 n
 * @idea 按整除块筛出质数的一次、二次幂和，再按最小质因子和指数递归计算合数贡献。
 * @usage min25_sum(n, mod, {c0,c1,c2}, f) 求前缀和。f(p,e,pe) 返回 f(p^e)，pe 就是 p^e。
 * @pitfall n 在 0 到 10^12，mod 为正 int。f(1)=1，f(p)=c0+c1*p+c2*p^2。需要 __int128 计算初值。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

template <class PrimePower>
int min25_sum(ll n, int mod, array<int, 3> coefficient, PrimePower f) {
    if (n == 0)
        return 0;
    using Wide = __int128;
    auto norm = [mod](ll x) { return (x % mod + mod) % mod; };
    int root = (int)sqrt((ld)n);
    while (root + 1 <= n / (root + 1))
        root++;
    while (root > n / root)
        root--;

    vector<ll> values;
    vector<int> small(root + 1), large(root + 1);
    vector<array<int, 3>> sum;
    for (ll l = 1; l <= n;) {
        ll x = n / l;
        int id = (int)values.size();
        values.push_back(x);
        if (x <= root)
            small[x] = id;
        else
            large[n / x] = id;
        // 三张表分别筛 1、p、p^2。初值从 2 开始求和。
        sum.push_back({(int)((x - 1) % mod),
                       (int)(((Wide)x * (x + 1) / 2 - 1) % mod),
                       (int)(((Wide)x * (x + 1) * (2 * x + 1) / 6 - 1) % mod)});
        l = n / x + 1;
    }
    auto index = [&](ll x) { return x <= root ? small[x] : large[n / x]; };

    vector<int> primes;
    vector<bool> composite(root + 1);
    for (int p = 2; p <= root; p++) {
        if (composite[p])
            continue;
        primes.push_back(p);
        if (p <= root / p)
            for (int j = p * p; j <= root; j += p)
                composite[j] = true;

        auto before = sum[index(p - 1)];
        array<ll, 3> factor{1, p % mod, 1LL * p * p % mod};
        for (int i = 0; i < (int)values.size() && values[i] >= 1LL * p * p; i++) {
            auto below = sum[index(values[i] / p)];
            for (int j = 0; j < 3; j++)
                sum[i][j] = norm(sum[i][j] - factor[j] * (below[j] - before[j]));
        }
    }

    vector<int> prime_sum(values.size());
    for (int i = 0; i < (int)values.size(); i++)
        for (int j = 0; j < 3; j++)
            prime_sum[i] = norm(prime_sum[i] + 1LL * coefficient[j] * sum[i][j]);
    vector<int> prefix(primes.size() + 1);
    for (int i = 0; i < (int)primes.size(); i++)
        prefix[i + 1] = norm(prefix[i] + norm(f(primes[i], 1, primes[i])));

    auto dfs = [&](auto &&self, ll x, int start) -> ll {
        ll result = norm(prime_sum[index(x)] - prefix[start]);
        for (int i = start; i < (int)primes.size(); i++) {
            ll p = primes[i];
            if (p > x / p)
                break;
            for (ll pe = p, e = 1; pe <= x / p; pe *= p, e++) {
                ll tail = self(self, x / pe, i + 1);
                result = (result + norm(f(p, e, pe)) * tail +
                          norm(f(p, e + 1, pe * p))) %
                         mod;
            }
        }
        return result;
    };
    return (dfs(dfs, n, 0) + 1) % mod;
}
