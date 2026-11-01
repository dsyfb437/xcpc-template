/**
 * @brief 中国剩余定理
 * @complexity O(k log M)
 *
 * 解 x ≡ r[i] (mod m[i])，要求 m[i] 两两互质，M = 所有 m[i] 的乘积。
 * 返回 {最小非负解, M}；不互质时 inv_mod 返回 -1，函数返回 {-1, 0}。
 * 例：
 *   auto [x, M] = crt({2, 3}, {3, 5});  // x = 8, M = 15
 */
#pragma once
#include <bits/stdc++.h>
#include "exgcd.hpp"
using namespace std;
using ll = long long;

inline pair<ll, ll> crt(const vector<ll> &r, const vector<ll> &m) {
    ll M = 1;
    for (ll x : m)
        M *= x;
    ll ans = 0;
    for (size_t i = 0; i < r.size(); i++) {
        ll Mi = M / m[i];
        ll inv = inv_mod(Mi % m[i], m[i]);
        if (inv < 0)
            return {-1, 0};
        ll term = (ll)((__int128)((r[i] % m[i] + m[i]) % m[i]) * Mi % M);
        ans = (ans + (ll)((__int128)term * inv % M)) % M;
    }
    return {ans, M};
}
