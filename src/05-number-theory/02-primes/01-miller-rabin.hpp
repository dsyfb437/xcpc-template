/**
 * @brief Miller–Rabin 素性测试
 * @complexity O(log n) 次模乘，固定 12 个底数
 * @index 无符号 64 位整数
 * @idea 把 n - 1 写成奇数乘 2 的幂，检查快速幂序列有没有经过 -1 或一开始就是 1。
 * @usage miller_rabin(n) 判断 n 是否为质数。
 * @pitfall 前 12 个素数底数对整个 uint64_t 范围确定正确。有 128 位乘法时优先使用。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace miller_rabin_detail {
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
} // namespace miller_rabin_detail

inline bool miller_rabin(ull n) {
    return miller_rabin_detail::prime(n);
}
