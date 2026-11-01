/**
 * @brief 树上路径交
 * @complexity 单次 O(log n)
 * @index 点 1-based
 *
 * 用途：求两条路径 (a,b)、(c,d) 的交。返回 {交的边数, 端点 x, 端点 y}；
 * 无交返回 {-1, 0, 0}。
 * 做法：交的端点必在 8 个候选点里（4 个原端点 + 4 个两两 LCA），
 * 筛出同时在两条路径上的点，取距离最远的一对。
 * 依赖 lca.hpp。
 *
 * 例：
 *   auto [len, x, y] = path_intersection(lca, a, b, c, d); */
#pragma once
#include <bits/stdc++.h>
#include "03-最近公共祖先.hpp"
using namespace std;

inline tuple<int, int, int> path_intersection(const LCA &lca, int a, int b, int c,
                                              int d) {
    auto on_path = [&](int x, int u, int v) {
        return lca.dist(u, x) + lca.dist(x, v) == lca.dist(u, v);
    };
    int cand[8] = {
        a, b, c, d, lca.lca(a, c), lca.lca(a, d), lca.lca(b, c), lca.lca(b, d)};
    vector<int> hit;
    for (int x : cand) {
        if (!on_path(x, a, b) || !on_path(x, c, d))
            continue;
        if (find(hit.begin(), hit.end(), x) != hit.end())
            continue;
        hit.push_back(x);
    }
    if (hit.empty())
        return {-1, 0, 0};

    int best = -1, x = hit[0], y = hit[0];
    for (int i = 0; i < (int)hit.size(); i++)
        for (int j = i; j < (int)hit.size(); j++) {
            int d2 = lca.dist(hit[i], hit[j]);
            if (d2 > best) {
                best = d2;
                x = hit[i];
                y = hit[j];
            }
        }
    return {best, x, y};
}
