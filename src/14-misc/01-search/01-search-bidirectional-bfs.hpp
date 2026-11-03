/**
 * @brief 双向 BFS
 * @complexity 显式图最坏 O(n+m) 时间，O(n) 额外空间
 * @index 顶点 0-based
 * @idea 从起点沿原图搜，从终点沿反图搜。每次完整扩展较小的一层，第一次碰到对侧访问过的点就能拼出最短路。
 * @usage auto path = bidirectional_bfs(g, reverse_g, start, goal);
 *   路径含起终点，不可达返回空数组。
 * @pitfall 边权都为 1。无向图时两个图都传 g。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> bidirectional_bfs(const vector<vector<int>> &g,
                                     const vector<vector<int>> &reverse_g,
                                     int start, int goal) {
    if (start == goal)
        return {start};
    vector<int> front = {start}, back = {goal};
    vector<int> from_start(g.size(), -1), from_goal(g.size(), -1);
    from_start[start] = start;
    from_goal[goal] = goal;

    auto expand = [](vector<int> &layer, const auto &adj, vector<int> &parent,
                     const vector<int> &other) {
        vector<int> next;
        for (int u : layer) {
            for (int v : adj[u]) {
                if (parent[v] != -1)
                    continue;
                parent[v] = u;
                if (other[v] != -1)
                    return v;
                next.push_back(v);
            }
        }
        layer.swap(next);
        return -1;
    };

    while (!front.empty() && !back.empty()) {
        int meet = front.size() <= back.size()
                       ? expand(front, g, from_start, from_goal)
                       : expand(back, reverse_g, from_goal, from_start);
        if (meet == -1)
            continue;

        vector<int> path;
        for (int u = meet; u != start; u = from_start[u])
            path.push_back(u);
        path.push_back(start);
        reverse(path.begin(), path.end());
        for (int u = meet; u != goal;) {
            u = from_goal[u];
            path.push_back(u);
        }
        return path;
    }
    return {};
}
