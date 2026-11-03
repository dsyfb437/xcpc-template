/**
 * @brief 形式幂级数初等函数
 * @complexity O(n log n + log k)
 * @index 系数低次在前，保留前 n 项
 * @idea 整数幂先分离 x 的幂和首项，再用 exp(k ln f)。sin 和 cos 从 exp(i f) 与 exp(-i f) 求出。
 * @usage fps_power(f,k,n) 求非负整数幂。fps_sin_cos(f,n) 返回 {sin(f),cos(f)}。
 * @pitfall 模 998244353，系数先归约到 [0,998244353)。sin/cos 要求 f[0]=0，整数幂约定零次幂为 1。NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace fps_elementary_detail {
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

inline vector<int> log_series(const vector<int> &f, int n) {
    if (n == 0)
        return {};
    assert(!f.empty() && f[0] == 1);
    int len = min((int)f.size(), n);
    vector<int> derivative(len - 1);
    for (int i = 1; i < len; i++)
        derivative[i - 1] = 1LL * f[i] * i % mod;
    auto product = multiply(derivative, inverse_series(f, n));
    product.resize(n - 1);
    vector<int> result(n), inv(n);
    if (n > 1)
        inv[1] = 1;
    for (int i = 2; i < n; i++)
        inv[i] = mod - 1LL * (mod / i) * inv[mod % i] % mod;
    for (int i = 1; i < n; i++)
        result[i] = 1LL * product[i - 1] * inv[i] % mod;
    return result;
}

inline vector<int> exp_series(const vector<int> &f, int n) {
    if (n == 0)
        return {};
    assert(f.empty() || f[0] == 0);
    vector<int> g{1};
    for (int size = 1; size < n; size *= 2) {
        int len = min(2 * size, n);
        auto correction = log_series(g, len);
        for (int i = 0; i < len; i++) {
            int value = i < (int)f.size() ? f[i] : 0;
            correction[i] = (value - correction[i] + mod) % mod;
        }
        correction[0] = 1;
        g = multiply(g, correction);
        g.resize(len);
    }
    return g;
}
} // namespace fps_elementary_detail

inline vector<int> fps_power(const vector<int> &f, ull k, int n) {
    using namespace fps_elementary_detail;
    vector<int> result(n);
    if (n == 0)
        return result;
    if (k == 0) {
        result[0] = 1;
        return result;
    }
    int first = 0, limit = min((int)f.size(), n);
    while (first < limit && !f[first])
        first++;
    if (first == limit || (first && k > (unsigned)(n - 1) / first))
        return result;
    int offset = first ? first * k : 0;
    int len = n - offset, scale = power(f[first], mod - 2);
    vector<int> unit(f.begin() + first, f.begin() + min(limit, first + len));
    for (int &x : unit)
        x = 1LL * x * scale % mod;
    auto exponent = log_series(unit, len);
    for (int &x : exponent)
        x = (k % mod) * x % mod;
    auto value = exp_series(exponent, len);
    int leading = power(f[first], k);
    for (int i = 0; i < len; i++)
        result[i + offset] = 1LL * value[i] * leading % mod;
    return result;
}

inline pair<vector<int>, vector<int>> fps_sin_cos(vector<int> f, int n) {
    using namespace fps_elementary_detail;
    if (n == 0)
        return {};
    f.resize(min((int)f.size(), n));
    int imaginary = power(3, (mod - 1) / 4);
    for (int &x : f)
        x = 1LL * x * imaginary % mod;
    auto a = exp_series(f, n);
    auto b = inverse_series(a, n);
    constexpr int half = (mod + 1) / 2;
    int scale = power(2LL * imaginary % mod, mod - 2);
    vector<int> sine(n), cosine(n);
    for (int i = 0; i < n; i++) {
        sine[i] = 1LL * (a[i] - b[i] + mod) * scale % mod;
        cosine[i] = 1LL * (a[i] + b[i]) * half % mod;
    }
    return {move(sine), move(cosine)};
}
