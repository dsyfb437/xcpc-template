/**
 * @brief A* 最短路
 * @complexity 复杂度取决于估价。估价一致时 O(n+m log(2+m)) 时间
 * @index 顶点 0-based
 * @idea 优先取 g+h 最小的点，g 是已走距离，h 是到终点的估价。遇到更短的走法会重新入堆。
 * @usage auto answer = astar_path(g, start, goal, estimate);
 *   g[u] 每项是 {v, 边权}，有解时返回 {距离, 路径}。
 * @pitfall 边权非负，h 不得高估且 h(goal)=0。距离和 g+h 都要能放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Estimate>
optional<pair<ll, vector<int>>> astar_path(const vector<vector<pair<int, ll>>> &g,
                                           int start, int goal, Estimate estimate) {
    vector<ll> dist(g.size(), -1);
    vector<int> parent(g.size(), -1);
    // 堆键为 {估计总距离, 顶点, 入堆时的实际距离}。
    using Entry = pair<ll, pair<int, ll>>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> queue;
    dist[start] = 0;
    queue.push({estimate(start), {start, 0}});
    while (!queue.empty()) {
        auto [priority, step] = queue.top();
        queue.pop();
        auto [u, walked] = step;
        if (walked != dist[u])
            continue;
        if (u == goal) {
            vector<int> path;
            for (int v = goal; v != -1; v = parent[v])
                path.push_back(v);
            reverse(path.begin(), path.end());
            return pair{walked, move(path)};
        }
        for (auto [v, weight] : g[u]) {
            ll next = walked + weight;
            if (dist[v] != -1 && next >= dist[v])
                continue;
            dist[v] = next;
            parent[v] = u;
            queue.push({next + estimate(v), {v, next}});
        }
    }
    return nullopt;
}
