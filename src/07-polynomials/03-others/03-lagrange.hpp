/**
 * @brief 拉格朗日插值
 * @complexity 任意点 O(n^2 + n log mod)，连续点 O(n + log mod)
 * @index 样本下标 0-based
 * @idea 每个样本乘上在它处为 1、其他样本处为 0 的基多项式。横坐标连续时，分子用前后缀积，分母用阶乘。
 * @usage lagrange_eval(xs,ys,x,mod) 求任意样本的点值，lagrange_consecutive(ys,x,mod) 的横坐标固定为 0 到 n-1。
 * @pitfall mod 为 int 范围的素数，横坐标模 mod 后互异，所有输入先归约。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int lagrange_inverse(ll a, int mod) {
    ll result = 1;
    for (int e = mod - 2; e; e >>= 1, a = a * a % mod)
        if (e & 1)
            result = result * a % mod;
    return (int)result;
}

inline int lagrange_eval(const vector<int> &xs, const vector<int> &ys, int x,
                         int mod) {
    int n = (int)xs.size();
    ll answer = 0;
    for (int i = 0; i < n; i++) {
        if (x == xs[i])
            return ys[i];
        ll top = ys[i], bottom = 1;
        for (int j = 0; j < n; j++) {
            if (j == i)
                continue;
            top = top * (x - xs[j] + (ll)mod) % mod;
            bottom = bottom * (xs[i] - xs[j] + (ll)mod) % mod;
        }
        answer = (answer + top * lagrange_inverse(bottom, mod)) % mod;
    }
    return (int)answer;
}

inline int lagrange_consecutive(const vector<int> &ys, int x, int mod) {
    int n = (int)ys.size();
    if (n == 0)
        return 0;
    if (x < n)
        return ys[x];
    vector<int> left(n + 1, 1), right(n + 1, 1), inv(n);
    ll fact = 1;
    for (int i = 0; i < n; i++) {
        left[i + 1] = 1LL * left[i] * (x - i) % mod;
        if (i > 0)
            fact = fact * i % mod;
    }
    for (int i = n - 1; i >= 0; i--)
        right[i] = 1LL * right[i + 1] * (x - i) % mod;
    inv[n - 1] = lagrange_inverse(fact, mod);
    for (int i = n - 1; i > 0; i--)
        inv[i - 1] = 1LL * inv[i] * i % mod;

    ll answer = 0;
    for (int i = 0; i < n; i++) {
        ll term = 1LL * left[i] * right[i + 1] % mod * ys[i] % mod;
        term = term * inv[i] % mod * inv[n - 1 - i] % mod;
        answer = (answer + ((n - 1 - i) & 1 ? mod - term : term)) % mod;
    }
    return (int)answer;
}
