/**
 * @brief 扩展 BSGS（离散对数）
 * @complexity 期望 O(sqrt(m) + log m)
 * @index 指数从 0 起
 * @idea 先用 gcd 逐层约掉公因子。互质以后把指数分成大步和小步，查表找第一次相遇。
 * @usage discrete_log(a, b, m) 求 a^x = b mod m 的最小非负 x，无解返回 -1。
 * @pitfall m 是正 int，a、b 可以为负。约定 0^0 = 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll discrete_log(int a, int b, int m) {
    a %= m;
    b %= m;
    if (a < 0)
        a += m;
    if (b < 0)
        b += m;
    int offset = 0, factor = 1 % m;
    for (int g = gcd(a, m); g > 1; g = gcd(a, m)) {
        if (b == factor)
            return offset;
        if (b % g)
            return -1;
        b /= g;
        m /= g;
        factor = 1LL * factor * (a / g) % m;
        offset++;
    }
    if (m == 1)
        return offset;

    auto inverse = [m](ll u) {
        ll v = m, x = 1, y = 0;
        while (v) {
            ll q = u / v, r = u % v;
            u = v;
            v = r;
            ll z = x - q * y;
            x = y;
            y = z;
        }
        return (x % m + m) % m;
    };
    int step = (int)sqrt(m) + 1;
    unordered_map<int, int> baby;
    baby.reserve(step);
    ll power = 1;
    for (int j = 0; j < step; j++) {
        baby.try_emplace(power, j);
        power = power * a % m;
    }
    ll target = b * inverse(factor) % m;
    ll backward = inverse(power);
    for (int i = 0; i <= step; i++) {
        auto it = baby.find(target);
        if (it != baby.end())
            return offset + 1LL * i * step + it->second;
        target = target * backward % m;
    }
    return -1;
}
