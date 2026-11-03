/**
 * @brief 容斥原理（整除计数）
 * @complexity O(2^k log n)
 * @index 统计 1 到 n
 * @idea 选中的除数要同时整除，交集大小是 n 除以它们的最小公倍数。奇数项加，偶数项减。
 * @usage count_divisible(n, divisors) 返回至少被一个除数整除的整数个数。
 * @pitfall n 非负，除数都为正。适合 k 很小的情况，容斥中间和用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll count_divisible(ll n, const vector<ll> &divisors) {
    __int128 answer = 0;
    auto dfs = [&](auto &&self, int first, ll multiple, int sign) -> void {
        for (int i = first; i < (int)divisors.size(); i++) {
            ll d = divisors[i];
            ll reduced = multiple / gcd(multiple, d);
            if (reduced > n / d)
                continue;
            ll next = reduced * d;
            answer += sign * (n / next);
            self(self, i + 1, next, -sign);
        }
    };
    dfs(dfs, 0, 1, 1);
    return (ll)answer;
}
