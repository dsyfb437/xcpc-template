/**
 * @brief DAG 上的 DP（单源最长路）
 * @complexity O(n + m)
 * @index 点 0-based
 * @idea 按拓扑序处理边。到达 u 后，用当前最长路更新每条出边的终点。
 * @usage dag_longest_paths(g,s)，g[u] 存 {终点,边权}。返回各点的最长路。
 * @pitfall 边权可为负。若全图有环则整体返回 nullopt，不可达点对应的距离为空，距离用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<vector<optional<__int128>>>
dag_longest_paths(const vector<vector<pair<int, ll>>> &g, int s) {
    int n = (int)g.size();
    vector<int> indegree(n);
    for (const auto &edges : g)
        for (auto [v, w] : edges)
            indegree[v]++;
    queue<int> ready;
    for (int u = 0; u < n; u++)
        if (!indegree[u])
            ready.push(u);

    vector<optional<__int128>> distance(n);
    distance[s] = 0;
    int visited = 0;
    while (!ready.empty()) {
        int u = ready.front();
        ready.pop();
        visited++;
        for (auto [v, w] : g[u]) {
            if (distance[u]) {
                __int128 candidate = *distance[u] + w;
                if (!distance[v] || *distance[v] < candidate)
                    distance[v] = candidate;
            }
            if (--indegree[v] == 0)
                ready.push(v);
        }
    }
    if (visited != n)
        return nullopt;
    return distance;
}
