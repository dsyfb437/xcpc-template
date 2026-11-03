/**
 * @brief 多项式除法 / 取模
 * @complexity O(n log n)
 * @index 系数低次在前
 * @idea 先反转两式，把首项对齐的除法变成幂级数求逆。反转回去得到商，再用被除式减商乘除式得到余式。
 * @usage auto [q,r] = poly_divmod(a,b); 零多项式用空数组。
 * @pitfall 模 998244353，系数先归约到 [0,998244353)。除式非零，输入末尾的零会被去掉。NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace poly_divmod_detail {
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

inline pair<vector<int>, vector<int>> divide(vector<int> a, vector<int> b) {
    while (!a.empty() && a.back() == 0)
        a.pop_back();
    while (!b.empty() && b.back() == 0)
        b.pop_back();
    assert(!b.empty());
    if (a.size() < b.size())
        return {{}, move(a)};

    int len = (int)(a.size() - b.size() + 1);
    vector<int> ra(a.rbegin(), a.rend()), rb(b.rbegin(), b.rend());
    ra.resize(len);
    rb.resize(min((int)rb.size(), len));
    auto quotient = multiply(ra, inverse_series(rb, len));
    quotient.resize(len);
    reverse(quotient.begin(), quotient.end());

    auto product = multiply(quotient, b);
    a.resize(b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++)
        a[i] = (a[i] - product[i] + mod) % mod;
    while (!a.empty() && a.back() == 0)
        a.pop_back();
    return {move(quotient), move(a)};
}
} // namespace poly_divmod_detail

inline pair<vector<int>, vector<int>> poly_divmod(vector<int> a, vector<int> b) {
    return poly_divmod_detail::divide(move(a), move(b));
}
