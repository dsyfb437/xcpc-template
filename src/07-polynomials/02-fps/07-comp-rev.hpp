/**
 * @brief 形式幂级数复合 / 复合逆
 * @complexity 复合与复合逆 O(n^2)，空间 O(n sqrt(n))
 * @index 系数低次在前
 * @idea 按约 sqrt(n) 项分块，预处理 g 的小幂，再把 g^块长当成外层变量做 Horner。复合逆用牛顿法解 f(g)=x。
 * @usage fps_compose(f,g,n) 求 f(g(x))。fps_revert(f,n) 求复合逆。
 * @pitfall 模 998244353，系数先归约到 [0,998244353)。内层 g[0]=0，求逆时 f[0]=0 且 f[1] 非零。块内直接做线性组合，适合中等规模。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace fps_comp_detail {
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

inline vector<int> compose(const vector<int> &f, const vector<int> &g, int n) {
    if (n == 0)
        return {};
    assert(g.empty() || g[0] == 0);
    int count = min((int)f.size(), n);
    if (count == 0)
        return vector<int>(n);
    int block = (int)sqrt(count) + 1;
    vector<int> inner(g.begin(), g.begin() + min((int)g.size(), n));
    vector<vector<int>> baby(block + 1);
    baby[0] = {1};
    for (int i = 1; i <= block; i++) {
        baby[i] = multiply(baby[i - 1], inner);
        if ((int)baby[i].size() > n)
            baby[i].resize(n);
    }
    vector<int> answer;
    for (int start = (count - 1) / block * block; start >= 0; start -= block) {
        answer = multiply(move(answer), baby[block]);
        answer.resize(n);
        for (int j = 0; j < block && start + j < count; j++)
            if (int coefficient = f[start + j])
                for (int i = j; i < (int)baby[j].size(); i++)
                    answer[i] = (answer[i] + 1LL * coefficient * baby[j][i]) % mod;
    }
    return answer;
}
} // namespace fps_comp_detail

inline vector<int> fps_compose(const vector<int> &f, const vector<int> &g, int n) {
    return fps_comp_detail::compose(f, g, n);
}

inline vector<int> fps_revert(const vector<int> &f, int n) {
    using namespace fps_comp_detail;
    if (n <= 1)
        return vector<int>(n);
    assert(f.size() >= 2 && f[0] == 0 && f[1] != 0);
    vector<int> g{0, power(f[1], mod - 2)};
    vector<int> derivative(min((int)f.size() - 1, n));
    for (int i = 0; i < (int)derivative.size(); i++)
        derivative[i] = 1LL * (i + 1) * f[i + 1] % mod;
    for (int size = 2; size < n; size *= 2) {
        int len = min(2 * size, n);
        auto error = compose(f, g, len);
        error[1] = (error[1] + mod - 1) % mod;
        auto slope = compose(derivative, g, len);
        auto delta = multiply(error, inverse_series(slope, len));
        g.resize(len);
        for (int i = 0; i < len; i++)
            g[i] = (g[i] - delta[i] + mod) % mod;
    }
    return g;
}
