/**
 * @brief 旋转卡壳（凸包直径）
 * @complexity O(n)
 * @index 凸包下标 0-based
 * @idea 沿凸包枚举一条边，反对点随边单调向前。用叉积比较到边的距离，并检查平行边产生的并列。
 * @usage convex_diameter2(hull) 返回最远点对的距离平方。
 * @pitfall 输入为逆时针凸包，不重复首点，不保留边上中间点。整数坐标绝对值不超过 10^12，结果用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline __int128 convex_diameter2(const vector<pair<ll, ll>> &h) {
    int n = (int)h.size();
    if (n < 2)
        return 0;
    auto distance = [&](int i, int j) {
        __int128 x = (__int128)h[i].first - h[j].first;
        __int128 y = (__int128)h[i].second - h[j].second;
        return x * x + y * y;
    };
    if (n == 2)
        return distance(0, 1);

    auto area = [&](int i, int next, int j) {
        auto [x, y] = h[i];
        auto [a, b] = h[next];
        auto [c, d] = h[j];
        return (__int128)(a - x) * (d - y) - (__int128)(b - y) * (c - x);
    };
    __int128 best = 0;
    for (int i = 0, j = 1; i < n; i++) {
        int next = (i + 1) % n;
        while (area(i, next, (j + 1) % n) > area(i, next, j))
            j = (j + 1) % n;
        best = max({best, distance(i, j), distance(next, j)});
        int k = (j + 1) % n;
        if (area(i, next, k) == area(i, next, j))
            best = max({best, distance(i, k), distance(next, k)});
    }
    return best;
}
