/**
 * @brief Lagrange 反演（复合逆的单项系数）
 * @complexity O(n log^2 n)
 * @index n 从 0 起
 * @idea 若 g 是 f 的复合逆，则 g 的第 n 项等于 (x/f)^n 的第 n-1 项除以 n。用截断乘法求幂。
 * @usage inverse_composition_coefficient(f,n) 返回复合逆的第 n 项。
 * @pitfall 模 998244353，系数先归约到 [0,998244353)。f[0]=0 且 f[1] 非零。只取一个系数，NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace lagrange_inversion_detail {
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
} // namespace lagrange_inversion_detail

inline int inverse_composition_coefficient(const vector<int> &f, int n) {
    using namespace lagrange_inversion_detail;
    if (!n)
        return 0;
    assert(f.size() >= 2 && f[0] == 0 && f[1] != 0 && n < mod);
    vector<int> unit(f.begin() + 1, f.begin() + min((int)f.size(), n + 1));
    auto base = inverse_series(unit, n);
    vector<int> result{1};
    for (int e = n; e;) {
        if (e & 1) {
            result = multiply(result, base);
            result.resize(n);
        }
        e >>= 1;
        if (e) {
            base = multiply(base, base);
            base.resize(n);
        }
    }
    return 1LL * result[n - 1] * power(n, mod - 2) % mod;
}
