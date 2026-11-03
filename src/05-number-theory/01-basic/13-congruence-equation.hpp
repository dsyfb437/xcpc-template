/**
 * @brief 一元线性同余方程
 * @complexity O(log m)
 * @index 最小解从 0 起
 * @idea 令 g = gcd(a, m)。b 是 g 的倍数时约掉 g，再用扩欧求逆。
 * @usage linear_congruence(a, b, m) 解 a x ≡ b mod m，返回 {最小非负解, 周期}。
 * @pitfall m 为正数，a、b 可以为负，无解返回 nullopt。中间乘法用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<pair<ll, ll>> linear_congruence(ll a, ll b, ll m) {
    if (m <= 0)
        return nullopt;
    ll u = a % m, v = m;
    if (u < 0)
        u += m;
    __int128 x = 1, y = 0;
    while (v) {
        ll q = u / v, rem = u % v;
        __int128 z = x - q * y;
        u = v;
        v = rem;
        x = y;
        y = z;
    }
    if (b % u)
        return nullopt;
    ll step = m / u;
    ll answer = (ll)((b / u % step) * x % step);
    if (answer < 0)
        answer += step;
    return pair{answer, step};
}
