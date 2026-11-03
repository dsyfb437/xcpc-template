/**
 * @brief 简单多边形三角剖分（剪耳）
 * @complexity O(n^2)
 * @index 返回原始顶点下标 0-based
 * @idea 找到凸顶点，其相邻三点形成的三角形不包含任何凹点，就能剪掉。剪掉后只重算相邻两点。
 * @usage triangulate_polygon(p) 返回三角形的三个点编号，三角形为逆时针。
 * @pitfall 简单无孔格点多边形，允许顺或逆时针。顶点不重复、任意三点不共线，坐标绝对值不超过 10^12。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<array<int, 3>> triangulate_polygon(const vector<pair<ll, ll>> &p) {
    int n = (int)p.size();
    if (n < 3)
        return {};
    auto turn = [&](int a, int b, int c) {
        auto [x, y] = p[a];
        return (__int128)(p[b].first - x) * (p[c].second - y) -
               (__int128)(p[b].second - y) * (p[c].first - x);
    };
    __int128 area = 0;
    for (int i = 1; i + 1 < n; i++)
        area += turn(0, i, i + 1);
    int direction = area > 0 ? 1 : -1;
    vector<int> previous(n), next(n);
    vector<bool> alive(n, true), ear(n);
    for (int i = 0; i < n; i++) {
        previous[i] = (i - direction + n) % n;
        next[i] = (i + direction + n) % n;
    }
    auto is_ear = [&](int u) {
        int a = previous[u], b = next[u];
        if (turn(a, u, b) <= 0)
            return false;
        for (int v = 0; v < n; v++) {
            if (!alive[v] || v == a || v == u || v == b)
                continue;
            if (turn(previous[v], v, next[v]) > 0)
                continue;
            if (turn(a, u, v) >= 0 && turn(u, b, v) >= 0 && turn(b, a, v) >= 0)
                return false;
        }
        return true;
    };

    for (int i = 0; i < n; i++)
        ear[i] = is_ear(i);
    vector<array<int, 3>> result;
    for (int count = n; count > 2; count--) {
        int u = 0;
        while (u < n && (!alive[u] || !ear[u]))
            u++;
        if (u == n)
            return {};
        int a = previous[u], b = next[u];
        result.push_back({a, u, b});
        alive[u] = false;
        next[a] = b;
        previous[b] = a;
        ear[a] = is_ear(a);
        ear[b] = is_ear(b);
    }
    return result;
}
