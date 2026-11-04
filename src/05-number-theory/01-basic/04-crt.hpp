/**
 * @brief 中国剩余定理
 * @complexity O(k log M)
 * @index 方程编号 0-based
 * @idea 解 x ≡ r[i] mod m[i]，模数两两互质时，把每个同余式的贡献加起来。
 * @usage auto [x, M] = crt({2, 3}, {3, 5}); 得到 x = 8、周期 M = 15。
 * @pitfall 余数允许为负。模数须为正且两两互质，乘积须放进 long long；不满足条件或两个数组长度不同返回 {-1, 0}。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

ll inv_mod(ll a, ll m) {
    if (m <= 0)
        return -1;
    a %= m;
    if (a < 0)
        a += m;
    ll x, y;
    if (exgcd(a, m, x, y) != 1)
        return -1;
    x %= m;
    return x < 0 ? x + m : x;
}

inline pair<ll, ll> crt(const vector<ll> &r, const vector<ll> &m) {
    if (r.size() != m.size())
        return {-1, 0};
    ll M = 1;
    for (ll x : m) {
        if (x <= 0 || M > LLONG_MAX / x)
            return {-1, 0};
        M *= x;
    }
    ll ans = 0;
    for (size_t i = 0; i < r.size(); i++) {
        ll Mi = M / m[i];
        ll inv = inv_mod(Mi % m[i], m[i]);
        if (inv < 0)
            return {-1, 0};
        ll residue = r[i] % m[i];
        if (residue < 0)
            residue += m[i];
        ll term = (ll)((__int128)residue * Mi % M);
        ans = (ll)((ans + (__int128)term * inv) % M);
    }
    return {ans, M};
}
