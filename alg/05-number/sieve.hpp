/**
 * @brief 线性筛
 * @complexity O(n)
 * @index 下标 1-based（1..n）
 *
 * 一遍筛出素数、最小质因子 lp、欧拉函数 phi、莫比乌斯函数 mu。
 * 下标从 1 开始，Sieve(n) 之后 primes 里是 <= n 的素数。
 *
 * 例：
 *   Sieve sv(n);
 *   sv.primes;    // <= n 的素数
 *   sv.phi[i];    // 欧拉函数
 *   sv.mu[i];     // 莫比乌斯函数 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Sieve {
    vector<int> primes, lp, phi, mu;

    explicit Sieve(int n) : lp(n + 1, 0), phi(n + 1, 0), mu(n + 1, 0) {
        if (n >= 1)
            phi[1] = 1, mu[1] = 1;
        for (int i = 2; i <= n; i++) {
            if (!lp[i]) {
                lp[i] = i;
                primes.push_back(i);
                phi[i] = i - 1;
                mu[i] = -1;
            }
            for (int p : primes) {
                if (1LL * i * p > n)
                    break;
                lp[i * p] = p;
                if (i % p == 0) {
                    phi[i * p] = phi[i] * p;
                    mu[i * p] = 0;
                    break;
                }
                phi[i * p] = phi[i] * (p - 1);
                mu[i * p] = -mu[i];
            }
        }
    }
};
