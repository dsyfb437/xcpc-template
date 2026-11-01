/**
 * @brief 最小生成树（Kruskal）
 * @complexity O(m log m)
 * @index 点 1-based，边下标 0-based
 *
 * 用途：无向带权图的最小生成树，返回总权值和选中的边下标。
 * 用法：`auto [cost, ids] = kruskal(n, edges);`，不连通时 cost = -1。
 * 边结构 {u, v, w, id}，点 1-indexed；自带迷你并查集，不依赖数据结构章。
 *
 * 例：
 *   vector<Edge> es = {{u, v, id, w}, ...};
 *   auto [cost, ids] = kruskal(n, es); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge {
    int u, v, id;
    ll w;
};

// 返回 {最小生成树总权值, 选中的边下标}；不连通返回 {-1, {}}。
inline pair<ll, vector<int>> kruskal(int n, vector<Edge> edges) {
    sort(edges.begin(), edges.end(),
         [](const Edge &a, const Edge &b) { return a.w < b.w; });

    vector<int> p(n + 1), sz(n + 1, 1);
    iota(p.begin(), p.end(), 0);
    auto find = [&](int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    };

    ll cost = 0;
    vector<int> chosen;
    for (const Edge &e : edges) {
        int a = find(e.u), b = find(e.v);
        if (a == b)
            continue;
        if (sz[a] < sz[b])
            swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        cost += e.w;
        chosen.push_back(e.id);
        if ((int)chosen.size() == n - 1)
            break;
    }
    if ((int)chosen.size() != n - 1)
        return {-1, {}};
    return {cost, chosen};
}
