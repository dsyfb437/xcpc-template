/**
 * @brief 符号化方法（SEQ 与 MSET）
 * @complexity O(n log n)
 * @index 系数按对象大小排列
 * @idea 序列类的 OGF 是 1/(1-A)。无序可重复组合的 OGF 是 exp(sum A(x^k)/k)。
 * @usage sequence_ogf(a,n) 与 multiset_ogf(a,n) 分别返回前 n 项。a[i] 数大小为 i 的原子种类。
 * @pitfall 模 998244353，系数先归约到 [0,998244353)。a[0] 必须为 0，以免大小 0 有无限多种。NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace symbolic_detail {
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
} // namespace symbolic_detail

inline vector<int> sequence_ogf(const vector<int> &a, int n) {
    using namespace symbolic_detail;
    if (!n)
        return {};
    assert(a.empty() || a[0] == 0);
    vector<int> denominator(min((int)a.size(), n));
    if (denominator.empty())
        denominator.resize(1);
    denominator[0] = 1;
    for (int i = 1; i < (int)denominator.size(); i++)
        denominator[i] = (mod - a[i]) % mod;
    return inverse_series(denominator, n);
}

inline vector<int> multiset_ogf(const vector<int> &a, int n) {
    using namespace symbolic_detail;
    if (!n)
        return {};
    assert(a.empty() || a[0] == 0);
    vector<int> logarithm(n), inv(n);
    if (n > 1)
        inv[1] = 1;
    for (int k = 2; k < n; k++)
        inv[k] = mod - 1LL * (mod / k) * inv[mod % k] % mod;
    for (int k = 1; k < n; k++)
        for (int i = 1; i < (int)a.size() && i <= (n - 1) / k; i++)
            logarithm[i * k] = (logarithm[i * k] + 1LL * a[i] * inv[k]) % mod;
    return exp_series(logarithm, n);
}
