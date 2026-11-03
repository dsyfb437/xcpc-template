/**
 * @brief 扩展中国剩余定理
 * @complexity O(k log M)
 * @index 方程编号 0-based
 * @idea 逐个合并 x = r mod M 和 x = a mod m。解 M t = a - r mod m，再用最小公倍数作新周期。
 * @usage extended_crt(equations) 返回 {最小非负解, 周期}，输入 {余数, 正模数}。
 * @pitfall 模数可以不互质。无解、非法模数或周期超过 long long 时返回 nullopt。需要 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<pair<ll, ll>> extended_crt(const vector<pair<ll, ll>> &equations) {
    ll r = 0, period = 1;
    for (auto [a, m] : equations) {
        if (m <= 0)
            return nullopt;
        ll u = period, v = m;
        __int128 x = 1, y = 0;
        while (v) {
            ll q = u / v, rem = u % v;
            __int128 next = x - q * y;
            u = v;
            v = rem;
            x = y;
            y = next;
        }

        __int128 delta = (__int128)a - r;
        if (delta % u != 0 || period > LLONG_MAX / (m / u))
            return nullopt;
        ll cycle = m / u;
        __int128 step = (delta / u % cycle) * x % cycle;
        if (step < 0)
            step += cycle;
        r += (ll)(step * period);
        period *= cycle;
    }
    return pair{r, period};
}
