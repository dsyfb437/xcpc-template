/**
 * @brief 快速幂
 * @complexity O(log b)
 *
 * 求 a^b mod p。b 可以到 1e18；p 任意，p = 1 时结果 0。
 * 中间乘法用 __int128，p 到 1e18 也不会溢出。
 *
 * 例：
 *   ll r = mod_pow(a, b, 1000000007);  // a^b mod p */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll mod_pow(ll a, ll b, ll p) {
    a %= p;
    if (a < 0)
        a += p;
    ll r = 1 % p;
    while (b) {
        if (b & 1)
            r = (ll)((__int128)r * a % p);
        a = (ll)((__int128)a * a % p);
        b >>= 1;
    }
    return r;
}
