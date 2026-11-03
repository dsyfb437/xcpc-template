/**
 * @brief 整数线段相交
 * @complexity O(1)
 * @index 端点包含在内
 * @idea 叉积判断两端是否跨过另一条线。四点共线时改看字典序区间的交。
 * @usage segment_intersection(a,b,c,d) 返回 0 不交、1 交一点、2 重叠一段。
 * @pitfall 点用 pair<long long,long long>，坐标绝对值不超过 10^12。允许零长度线段。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int segment_intersection(pair<ll, ll> a, pair<ll, ll> b, pair<ll, ll> c,
                                pair<ll, ll> d) {
    auto turn = [](auto p, auto q, auto r) {
        auto [x, y] = p;
        __int128 cross = (__int128)(q.first - x) * (r.second - y) -
                         (__int128)(q.second - y) * (r.first - x);
        return (cross > 0) - (cross < 0);
    };
    int x = turn(a, b, c), y = turn(a, b, d);
    int z = turn(c, d, a), w = turn(c, d, b);
    if (x || y || z || w)
        return x * y <= 0 && z * w <= 0;

    auto left = max(min(a, b), min(c, d));
    auto right = min(max(a, b), max(c, d));
    if (left > right)
        return 0;
    return left == right ? 1 : 2;
}
