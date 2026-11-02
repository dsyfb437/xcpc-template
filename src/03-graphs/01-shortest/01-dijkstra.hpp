/**
 * @brief Dijkstra（堆优化）
 * @complexity O((n + m) log n)
 * @index 点 1-based
 *
 * @idea 邻接表 g 从 1 开始存，(终点, 边权)。不可达返回 INF。
 * @usage 例：`vector<ll> d = dijkstra(g, s);  // 不可达是 INF`
 * @pitfall 只适用于非负边权；有负边换 SPFA 或 Johnson。
 * 
 * @verified
 *  */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = LLONG_MAX / 4;

vector<ll> dijkstra(const vector<vector<pair<int, ll>>> &g, int s) {
    int n = g.size() - 1;
    vector<ll> d(n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    d[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du != d[u])
            continue; // 跳过过期堆项
        for (auto [v, w] : g[u]) {
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
        }
    }
    return d;
}
