/**
 * @brief 差分约束
 * @complexity O(nm)
 * @index 变量 1-based
 * @idea x[v] - x[u] <= w 对应 u 到 v 的权 w 边。所有初值设为 0，相当于加了超级源。
 * @usage `auto x = difference_constraints(n, constraints);` 每条约束用 {u, v, w} 表示，无解返回空数组。
 * @pitfall 可以有负边。松弛中的距离及加法结果需放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> difference_constraints(int n,
                                         const vector<tuple<int, int, ll>> &edges) {
    vector<ll> x(n + 1);
    for (int round = 0; round < n; round++) {
        bool changed = false;
        for (auto [u, v, w] : edges) {
            if (x[v] > x[u] + w) {
                x[v] = x[u] + w;
                changed = true;
            }
        }
        if (!changed)
            return x;
    }
    return n == 0 ? x : vector<ll>{};
}
