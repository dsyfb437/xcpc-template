/**
 * @brief 扩展欧几里得 / 逆元
 * @complexity O(log min(a, b))
 *
 * exgcd 要求 a、b 非负，返回 gcd(a, b)，并解出 a*x + b*y = gcd(a, b)。
 * inv_mod 的 a 可以为负，m 必须为正；没有逆元或模数非法时返回 -1。
 *
 * 例：
 *   ll x, y;
 *   ll g = exgcd(a, b, x, y);      // a*x + b*y = g
 *   ll inv = inv_mod(a, m);        // 没有逆元返回 -1 */
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
