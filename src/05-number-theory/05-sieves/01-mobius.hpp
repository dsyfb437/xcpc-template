/**
 * @brief 莫比乌斯反演
 * @complexity 筛 O(n)，反演 O(n log n)
 * @index 下标 1 到 n
 * @idea 线性筛按最小质因子递推 mu。已知 F[x] 是 f 的约数和时，对 F 和 mu 做狄利克雷卷积还原 f。
 * @usage mobius_table(n) 求 mu 表，mobius_invert(F) 还原约数和变换。
 * @pitfall F[0] 留空。反演加减的中间值须放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> mobius_table(int n) {
    vector<int> mu(n + 1), least(n + 1), primes;
    if (n >= 1)
        mu[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!least[i]) {
            least[i] = i;
            mu[i] = -1;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > n / i)
                break;
            least[i * p] = p;
            if (i % p == 0)
                break;
            mu[i * p] = -mu[i];
        }
    }
    return mu;
}

inline vector<ll> mobius_invert(const vector<ll> &sum) {
    int n = (int)sum.size() - 1;
    auto mu = mobius_table(n);
    vector<ll> f(n + 1);
    for (int d = 1; d <= n; d++)
        if (mu[d])
            for (int k = 1; k <= n / d; k++) {
                if (mu[d] == 1)
                    f[d * k] += sum[k];
                else
                    f[d * k] -= sum[k];
            }
    return f;
}
