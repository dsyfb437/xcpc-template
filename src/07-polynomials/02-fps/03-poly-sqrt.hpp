/**
 * @brief 形式幂级数开平方
 * @complexity O(n log n)
 * @index 系数低次在前，保留前 n 项
 * @idea 先去掉偶数次的前导零。首项开平方后，用 g = (g + f/g)/2 倍增。
 * @usage fps_sqrt(f,n) 返回一个平方根，无解返回 nullopt。
 * @pitfall 模 998244353，系数先归约到 [0,998244353)。首非零项选较小的标量平方根。截断后未确定的高次项补 0。NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace fps_sqrt_detail {
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

inline int scalar_sqrt(int a) {
    if (power(a, (mod - 1) / 2) != 1)
        return -1;
    constexpr int odd = (mod - 1) >> 23;
    int s = 23, c = power(3, odd);
    int x = power(a, (odd + 1) / 2), t = power(a, odd);
    while (t != 1) {
        int i = 0;
        for (int v = t; v != 1; v = 1LL * v * v % mod)
            i++;
        int b = power(c, 1U << (s - i - 1));
        x = 1LL * x * b % mod;
        c = 1LL * b * b % mod;
        t = 1LL * t * c % mod;
        s = i;
    }
    return min(x, mod - x);
}
} // namespace fps_sqrt_detail

inline optional<vector<int>> fps_sqrt(const vector<int> &f, int n) {
    using namespace fps_sqrt_detail;
    int first = 0, limit = min((int)f.size(), n);
    while (first < limit && f[first] == 0)
        first++;
    if (first == limit)
        return vector<int>(n);
    if (first & 1)
        return nullopt;
    int initial = scalar_sqrt(f[first]);
    if (initial == -1)
        return nullopt;

    vector<int> shifted(f.begin() + first, f.begin() + limit);
    vector<int> g{initial};
    for (int size = 1; size < n - first; size *= 2) {
        int len = min(2 * size, n - first);
        vector<int> prefix(shifted.begin(),
                           shifted.begin() + min((int)shifted.size(), len));
        auto quotient = multiply(prefix, inverse_series(g, len));
        quotient.resize(len);
        g.resize(len);
        for (int i = 0; i < len; i++)
            g[i] = 1LL * (g[i] + quotient[i]) * ((mod + 1) / 2) % mod;
    }
    vector<int> result(n);
    copy(g.begin(), g.end(), result.begin() + first / 2);
    return result;
}
