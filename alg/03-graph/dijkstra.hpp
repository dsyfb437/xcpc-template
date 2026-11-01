/**
 * @brief Dijkstra（堆优化）
 * @complexity O((n + m) log n)
 * @index 点 1-based
 *
 * 邻接表 g 从 1 开始存，(终点, 边权)。不可达返回 INF。
 * 只适用于非负边权；有负边换 SPFA 或 Johnson。
 *
 * 例：
 *   vector<ll> d = dijkstra(g, s);  // 不可达是 INF */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = LLONG_MAX / 4;

vector<ll> dijkstra(const vector<vector<pair<int, ll>>> &g, int s) {
    int n = g.size() - 1;
    vector<ll> d(n + 1, INF);
    vector<char> vis(n + 1, 0);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    d[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (vis[u])
            continue; // 出堆时才判重，松弛时判会错
        vis[u] = 1;
        for (auto [v, w] : g[u]) {
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
        }
    }
    return d;
}
