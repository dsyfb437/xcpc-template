/**
 * @brief 树的直径
 * @complexity O(n)
 * @index 点 1-based
 *
 * @idea 求树的直径（最远点对的距离）和两个端点。
 *       做法：从任一点 DFS 到最远点 u，再从 u DFS 到最远点 v。
 *       无权、带权（边权非负）各一个版本；邻接表 1-indexed。 
 * @usage `auto [len, u, v] = tree_diameter(g);`
 * @pitfall g 是非空连通树，路径长度要能放进返回值类型。边权必须非负，负权树用树形 DP。
 *
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;

// ---------- 无权 ----------

// 从 s 出发 DFS，返回 {最远点, 距离}
inline pii tree_far(const vector<vector<int>> &g, int s) {
    vector<int> dist(g.size(), 0);
    int far = s, best = 0;
    vector<pair<int, int>> stack{{s, 0}};
    while (!stack.empty()) {
        auto [u, p] = stack.back();
        stack.pop_back();
        if (dist[u] > best) {
            best = dist[u];
            far = u;
        }
        for (auto it = g[u].rbegin(); it != g[u].rend(); ++it) {
            int v = *it;
            if (v == p) continue;
            dist[v] = dist[u] + 1;
            stack.emplace_back(v, u);
        }
    }
    return {far, best};
}

// 返回 {直径, 端点 u, 端点 v}
inline tuple<int, int, int> tree_diameter(const vector<vector<int>> &g,
                                          int start = 1) {
    auto [u, _] = tree_far(g, start);
    auto [v, d] = tree_far(g, u);
    return {d, u, v};
}

// ---------- 带权（边权非负） ----------

// 从 s 出发 DFS，返回 {最远点, 距离}
inline pair<int, ll> tree_far(const vector<vector<pair<int, ll>>> &g, int s) {
    vector<ll> dist(g.size(), 0);
    int far = s;
    ll best = 0;
    vector<pair<int, int>> stack{{s, 0}};
    while (!stack.empty()) {
        auto [u, p] = stack.back();
        stack.pop_back();
        if (dist[u] > best) {
            best = dist[u];
            far = u;
        }
        for (auto it = g[u].rbegin(); it != g[u].rend(); ++it) {
            auto [v, w] = *it;
            if (v == p) continue;
            dist[v] = dist[u] + w;
            stack.emplace_back(v, u);
        }
    }
    return {far, best};
}

// 返回 {直径, 端点 u, 端点 v}
inline tuple<ll, int, int> tree_diameter(const vector<vector<pair<int, ll>>> &g,
                                         int start = 1) {
    auto [u, _] = tree_far(g, start);
    auto [v, d] = tree_far(g, u);
    return {d, u, v};
}
