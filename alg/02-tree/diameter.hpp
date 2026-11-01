/**
 * @brief 树的直径
 * @complexity O(n)
 * @index 点 1-based
 *
 * 用途：求树的直径（最远点对的距离）和两个端点。
 * 做法：从任一点 DFS 到最远点 u，再从 u DFS 到最远点 v。
 * 无权、带权（边权非负）各一个版本；邻接表 1-indexed。
 * 坑：边权必须非负；负权树用树形 DP。递归深度是树高，本地默认 8MB 栈会爆，
 *      测试前先 `ulimit -s unlimited`（DOMjudge 栈无限）。
 *
 * 例：
 *   auto [len, u, v] = tree_diameter(g);
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------- 无权 ----------

// 从 s 出发 DFS，返回 {最远点, 距离}
inline pair<int, int> tree_far(const vector<vector<int>> &g, int s) {
    vector<int> dist(g.size(), 0);
    int far = s, best = 0;
    auto dfs = [&](auto &&self, int u, int p) -> void {
        if (dist[u] > best) {
            best = dist[u];
            far = u;
        }
        for (int v : g[u])
            if (v != p) {
                dist[v] = dist[u] + 1;
                self(self, v, u);
            }
    };
    dfs(dfs, s, 0);
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

// 从 s 出发 DFS，par 顺带记父亲；返回 {最远点, 距离}
inline pair<int, ll> tree_far(const vector<vector<pair<int, ll>>> &g, int s,
                              vector<int> &par) {
    par.assign(g.size(), 0);
    vector<ll> dist(g.size(), 0);
    int far = s;
    ll best = 0;
    auto dfs = [&](auto &&self, int u, int p) -> void {
        if (dist[u] > best) {
            best = dist[u];
            far = u;
        }
        for (auto [v, w] : g[u])
            if (v != p) {
                par[v] = u;
                dist[v] = dist[u] + w;
                self(self, v, u);
            }
    };
    dfs(dfs, s, 0);
    return {far, best};
}

// 返回 {直径, 端点 u, 端点 v}
inline tuple<ll, int, int> tree_diameter(const vector<vector<pair<int, ll>>> &g,
                                         int start = 1) {
    vector<int> par;
    auto [u, _] = tree_far(g, start, par);
    auto [v, d] = tree_far(g, u, par);
    return {d, u, v};
}
