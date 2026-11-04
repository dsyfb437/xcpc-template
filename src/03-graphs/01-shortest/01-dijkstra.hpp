/**
 * @brief Dijkstra（堆优化）
 * @complexity O(n + m log(2 + m))
 * @index 点 1-based
 *
 * @idea 邻接表 g 从 1 开始存，(终点, 边权)。不可达返回 INF。
 * @usage 例：`vector<ll> d = dijkstra(g, s);  // 不可达是 INF`
 * @pitfall 边权非负。有限答案须小于 INF，更大的距离统一返回 INF。边权本身可以很大，松弛前会先判断是否超限。
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
            if (w >= INF - du)
                continue;
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
        }
    }
    return d;
}
