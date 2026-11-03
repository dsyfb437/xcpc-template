/**
 * @brief 形式幂级数牛顿迭代
 * @complexity 卷积部分 O(n log n)，另计回调
 * @index 系数低次在前，保留前 n 项
 * @idea 对 F(x,y)=0，从已知常数解开始，用 y -= F/F_y 修正。每轮把截断精度翻倍。
 * @usage fps_newton(y0,n,evaluate) 返回 y。evaluate(y,len) 返回 {F(x,y), F_y(x,y)}，均取前 len 项。
 * @pitfall 模 998244353，F(0,y0)=0 且 F_y(0,y0) 非零，回调系数须已归约。NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace fps_newton_detail {
inline constexpr int mod = 998244353;

inline int power(ll x, ull e) {
    ll result = 1;
    for (; e; e >>= 1, x = x * x % mod)
        if (e & 1)
            result = result * x % mod;
    return (int)result;
}

inline void ntt(vector<int> &a, bool inverse) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }
    int generator = inverse ? power(3, mod - 2) : 3;
    for (int len = 2; len <= n; len *= 2) {
        int root = power(generator, (mod - 1) / len);
        for (int i = 0; i < n; i += len) {
            ll w = 1;
            for (int j = 0; j < len / 2; j++) {
                int u = a[i + j], v = w * a[i + j + len / 2] % mod;
                a[i + j] = u + v >= mod ? u + v - mod : u + v;
                a[i + j + len / 2] = u >= v ? u - v : u - v + mod;
                w = w * root % mod;
            }
        }
    }
    if (inverse) {
        int inv = power(n, mod - 2);
        for (int &x : a)
            x = 1LL * x * inv % mod;
    }
}

inline vector<int> multiply(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty())
        return {};
    int size = (int)(a.size() + b.size() - 1);
    if (min(a.size(), b.size()) <= 32) {
        vector<int> answer(size);
        for (int i = 0; i < (int)a.size(); i++)
            for (int j = 0; j < (int)b.size(); j++)
                answer[i + j] = (answer[i + j] + 1LL * a[i] * b[j]) % mod;
        return answer;
    }
    int n = (int)bit_ceil((unsigned)size);
    assert(n <= (1 << 23));
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; i++)
        a[i] = 1LL * a[i] * b[i] % mod;
    ntt(a, true);
    a.resize(size);
    return a;
}

inline vector<int> inverse_series(const vector<int> &f, int n) {
    if (n == 0)
        return {};
    assert(!f.empty() && f[0] != 0);
    vector<int> g{power(f[0], mod - 2)};
    for (int size = 1; size < n; size *= 2) {
        int len = min(2 * size, n);
        vector<int> prefix(f.begin(), f.begin() + min((int)f.size(), len));
        auto correction = multiply(prefix, g);
        correction.resize(len);
        for (int &x : correction)
            if (x)
                x = mod - x;
        correction[0] = (correction[0] + 2) % mod;
        g = multiply(g, correction);
        g.resize(len);
    }
    return g;
}
} // namespace fps_newton_detail

template <class Evaluate> vector<int> fps_newton(int y0, int n, Evaluate evaluate) {
    using namespace fps_newton_detail;
    if (n == 0)
        return {};
    vector<int> y{y0};
    for (int size = 1; size < n; size *= 2) {
        int len = min(2 * size, n);
        auto [value, derivative] = evaluate(y, len);
        value.resize(len);
        derivative.resize(len);
        auto change = multiply(value, inverse_series(derivative, len));
        change.resize(len);
        y.resize(len);
        for (int i = 0; i < len; i++)
            y[i] = (y[i] - change[i] + mod) % mod;
    }
    return y;
}
