/**
 * @brief Pick 定理
 * @complexity O(n log C)，C 是坐标范围
 * @index 顶点按边界顺序
 * @idea 鞋带公式求两倍面积。每条边给边界点数贡献 gcd(abs(dx),abs(dy))，再用 Pick 定理反求内部点数。
 * @usage auto [inside, boundary] = pick_lattice_points(p); 返回严格内部点和边界点数。
 * @pitfall 输入是面积非零的无孔简单格点多边形，至少三个不同顶点。顺逆时针均可，首点可重复。坐标绝对值不超过 10^12。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<__int128, __int128> pick_lattice_points(const vector<pair<ll, ll>> &p) {
    __int128 area2 = 0, boundary = 0;
    for (int i = 0; i < (int)p.size(); i++) {
        auto [x, y] = p[i];
        auto [u, v] = p[(i + 1) % p.size()];
        area2 += (__int128)x * v - (__int128)y * u;
        boundary += gcd(llabs(u - x), llabs(v - y));
    }
    if (area2 < 0)
        area2 = -area2;
    return {(area2 - boundary + 2) / 2, boundary};
}
