/**
 * @brief OGF 与 EGF（阶乘缩放、二项卷积）
 * @complexity 换算 O(n + log mod)，卷积 O(n log n)
 * @index 数组下标是阶数
 * @idea 同一计数序列的 EGF 系数等于 OGF 系数除以阶乘。相乘后再乘回阶乘，就得到带二项式系数的卷积。
 * @usage ogf_to_egf(a)、egf_to_ogf(a) 换算。binomial_convolution(a,b) 求 sum C(n,i)*a[i]*b[n-i]。
 * @pitfall 模 998244353，阶数小于模数，输入先归约。NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace ogf_egf_detail {
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
} // namespace ogf_egf_detail

inline vector<int> ogf_to_egf(vector<int> a) {
    using namespace ogf_egf_detail;
    int n = (int)a.size();
    if (!n)
        return a;
    ll fact = 1;
    for (int i = 1; i < n; i++)
        fact = fact * i % mod;
    ll inverse = power(fact, mod - 2);
    for (int i = n - 1; i >= 0; i--) {
        a[i] = a[i] * inverse % mod;
        inverse = inverse * i % mod;
    }
    return a;
}

inline vector<int> egf_to_ogf(vector<int> a) {
    constexpr int mod = ogf_egf_detail::mod;
    ll fact = 1;
    for (int i = 1; i < (int)a.size(); i++) {
        fact = fact * i % mod;
        a[i] = a[i] * fact % mod;
    }
    return a;
}

inline vector<int> binomial_convolution(vector<int> a, vector<int> b) {
    return egf_to_ogf(
        ogf_egf_detail::multiply(ogf_to_egf(move(a)), ogf_to_egf(move(b))));
}
