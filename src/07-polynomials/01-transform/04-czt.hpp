/**
 * @brief Chirp Z 变换（等比数列多点求值）
 * @complexity O((n + m) log(n + m))
 * @index 结果下标 0 到 m - 1
 * @idea 将 q^(ij) 拆成三个三角指数因子，反转加权后的系数，一次卷积即可取出所有点值。
 * @usage chirp_z(f,a,q,m) 返回 f(a*q^j)，j 从 0 到 m - 1。
 * @pitfall 模 998244353，q 非零，a 可为零。输入系数和参数先归约，NTT 长度至多 2^23。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace chirp_z_detail {
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
} // namespace chirp_z_detail

inline vector<int> chirp_z(const vector<int> &f, int a, int q, int m) {
    using namespace chirp_z_detail;
    assert(q != 0);
    int n = (int)f.size();
    if (n == 0 || m == 0)
        return vector<int>(m);
    vector<int> left(n), kernel(n + m - 1);
    int qi = power(q, mod - 2);
    ll a_power = 1, weight = 1, step = 1;
    for (int i = 0; i < n; i++) {
        left[n - 1 - i] = f[i] * a_power % mod * weight % mod;
        a_power = a_power * a % mod;
        weight = weight * step % mod;
        step = step * qi % mod;
    }
    weight = step = 1;
    for (int &x : kernel) {
        x = weight;
        weight = weight * step % mod;
        step = step * q % mod;
    }
    auto product = multiply(move(left), move(kernel));
    vector<int> answer(m);
    weight = step = 1;
    for (int j = 0; j < m; j++) {
        answer[j] = product[n - 1 + j] * weight % mod;
        weight = weight * step % mod;
        step = step * qi % mod;
    }
    return answer;
}
