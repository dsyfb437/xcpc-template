/**
 * @brief Pollard–Rho 分解质因数
 * @complexity 随机，找质因子 p 期望 O(sqrt(p)) 次迭代
 * @index 无符号 64 位整数
 * @idea 随机迭代 x^2 + c，快慢指针的差对某个质因子先变成 0 时，gcd 能提取因子。
 * @usage pollard_factor(n, seed) 返回升序质因子表，保留重复项。
 * @pitfall n 至少为 1，n = 1 返回空表。分解耗时依赖随机轨迹，质数判定对 uint64_t 是确定的。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace pollard_detail {
inline ull add(ull a, ull b, ull mod) {
    return a >= mod - b ? a - (mod - b) : a + b;
}

inline ull multiply(ull a, ull b, ull mod) {
#ifdef __SIZEOF_INT128__
    return (unsigned __int128)a * b % mod;
#else
    ull result = 0;
    for (; b; b >>= 1, a = add(a, a, mod))
        if (b & 1)
            result = add(result, a, mod);
    return result;
#endif
}

inline bool prime(ull n) {
    if (n < 2)
        return false;
    constexpr ull bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (ull p : bases)
        if (n % p == 0)
            return n == p;

    int s = countr_zero(n - 1);
    ull d = (n - 1) >> s;
    for (ull a : bases) {
        ull x = 1;
        for (ull e = d; e; e >>= 1, a = multiply(a, a, n))
            if (e & 1)
                x = multiply(x, a, n);
        if (x == 1 || x == n - 1)
            continue;
        int r = 1;
        for (; r < s; r++) {
            x = multiply(x, x, n);
            if (x == n - 1)
                break;
        }
        if (r == s)
            return false;
    }
    return true;
}
} // namespace pollard_detail

inline vector<ull> pollard_factor(ull n, ull seed = random_device{}()) {
    using namespace pollard_detail;
    mt19937_64 rng(seed);
    vector<ull> factors;
    auto split = [&](auto &&self, ull value) -> void {
        if (value <= 1)
            return;
        if (prime(value)) {
            factors.push_back(value);
            return;
        }
        ull factor = 2;
        if (value % 2) {
            do {
                ull c = uniform_int_distribution<ull>(1, value - 1)(rng);
                ull x = uniform_int_distribution<ull>(0, value - 1)(rng);
                ull y = x;
                auto step = [&](ull a) {
                    return add(multiply(a, a, value), c, value);
                };
                do {
                    x = step(x);
                    y = step(step(y));
                    factor = gcd(x > y ? x - y : y - x, value);
                } while (factor == 1);
            } while (factor == value);
        }
        self(self, factor);
        self(self, value / factor);
    };
    split(split, n);
    sort(factors.begin(), factors.end());
    return factors;
}
