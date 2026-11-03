/**
 * @brief 欧拉函数
 * @complexity 单点 O(sqrt(n))，整表 O(n)
 * @index 整表下标 0 到 n
 * @idea 每个不同质因子 p 让答案乘上 (p - 1) / p。整表用线性筛逐个扩展最小质因子。
 * @usage euler_phi(n) 查单点，phi_table(n) 得到 1 到 n 的欧拉函数。
 * @pitfall 单点 n 为正整数，phi(1) = 1。整表的 phi[0] 留为 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll euler_phi(ll n) {
    ll answer = n;
    for (ll p = 2; p <= n / p; p++) {
        if (n % p)
            continue;
        answer -= answer / p;
        do {
            n /= p;
        } while (n % p == 0);
    }
    if (n > 1)
        answer -= answer / n;
    return answer;
}

inline vector<int> phi_table(int n) {
    vector<int> phi(n + 1), primes;
    if (n >= 1)
        phi[1] = 1;
    for (int u = 2; u <= n; u++) {
        if (phi[u] == 0) {
            primes.push_back(u);
            phi[u] = u - 1;
        }
        for (int p : primes) {
            if (p > n / u)
                break;
            if (u % p == 0) {
                phi[u * p] = phi[u] * p;
                break;
            }
            phi[u * p] = phi[u] * (p - 1);
        }
    }
    return phi;
}
