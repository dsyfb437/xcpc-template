/**
 * @brief 裴蜀定理（线性丢番图方程）
 * @complexity O(log max(a, b))
 * @index 整数解
 * @idea 扩欧先求 a x + b y = gcd(a, b)，右端能整除 gcd 才能按比例放大。
 * @usage bezout_solution(a, b, c) 返回一组解 (x, y)，或 nullopt。
 * @pitfall a、b 非负。宽整数用于保存系数和放大后的解，需 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<pair<__int128, __int128>> bezout_solution(ll a, ll b, ll c) {
    if (a == 0 && b == 0) {
        if (c != 0)
            return nullopt;
        return pair<__int128, __int128>{0, 0};
    }
    __int128 x = 1, y = 0, next_x = 0, next_y = 1;
    while (b) {
        ll q = a / b, r = a % b;
        a = b;
        b = r;
        __int128 u = x - q * next_x, v = y - q * next_y;
        x = next_x;
        y = next_y;
        next_x = u;
        next_y = v;
    }
    if (c % a)
        return nullopt;
    ll scale = c / a;
    return pair{x * scale, y * scale};
}
