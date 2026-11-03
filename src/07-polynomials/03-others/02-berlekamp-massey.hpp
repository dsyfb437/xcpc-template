/**
 * @brief Berlekamp–Massey（最短线性递推）
 * @complexity O(n^2 + n log mod)
 * @index 序列从 0 起
 * @idea 用已有递推预测下一项。误差非零时，用上次留下的误差多项式消去它，必要时提高递推阶数。
 * @usage berlekamp_massey(s,mod) 返回 c，满足 s[i]=c[0]*s[i-1]+...+c[k-1]*s[i-k]。
 * @pitfall mod 为素数，输入先归约。返回的是拟合已给前缀的最短递推，样本不足时外推不一定唯一。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> berlekamp_massey(const vector<int> &s, int mod) {
    auto inverse = [mod](ll x) {
        ll result = 1;
        for (int e = mod - 2; e; e >>= 1, x = x * x % mod)
            if (e & 1)
                result = result * x % mod;
        return result;
    };
    vector<int> current{1}, previous{1};
    int order = 0, offset = 1;
    ll last_inverse = 1;
    for (int i = 0; i < (int)s.size(); i++) {
        ll error = s[i];
        for (int j = 1; j <= order; j++)
            error = (error + 1LL * current[j] * s[i - j]) % mod;
        if (!error) {
            offset++;
            continue;
        }

        auto old = current;
        ll factor = error * last_inverse % mod;
        int need = (int)previous.size() + offset;
        if ((int)current.size() < need)
            current.resize(need);
        for (int j = 0; j < (int)previous.size(); j++)
            current[j + offset] =
                (current[j + offset] - factor * previous[j] % mod + mod) % mod;
        if (2 * order <= i) {
            order = i + 1 - order;
            previous = old;
            previous.resize(i + 2 - order);
            last_inverse = inverse(error);
            offset = 1;
        } else {
            offset++;
        }
    }
    vector<int> result(order);
    for (int j = 0; j < order; j++)
        result[j] = (mod - current[j + 1]) % mod;
    return result;
}
