/**
 * @brief 无向图点连通度与边连通度
 * @complexity 边割跑 O(n) 次最大流，点割跑 O(n^2) 次
 * @index 点 0-based
 * @idea 边割固定一个源点枚举汇点。点割枚举不相邻端点，拆点后给每个中间点容量 1。
 * @usage auto [vertex, edge] = graph_connectivity(n, edges); 返回两种全局连通度。
 * @pitfall 简单无向图，适合小图。完全图的点连通度是 n - 1。不连通图两种连通度都是 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct ConnectivityFlow {
    struct Arc {
        int to, reverse, capacity;
    };
    vector<vector<Arc>> g;
    vector<int> level, next;

    explicit ConnectivityFlow(int n) : g(n), level(n), next(n) {}

    void add(int u, int v, int cap) {
        int a = (int)g[u].size(), b = (int)g[v].size();
        g[u].push_back({v, b, cap});
        g[v].push_back({u, a, 0});
    }

    int send(int u, int t, int limit) {
        if (u == t)
            return limit;
        for (int &i = next[u]; i < (int)g[u].size(); i++) {
            auto &e = g[u][i];
            if (e.capacity == 0 || level[e.to] != level[u] + 1)
                continue;
            int pushed = send(e.to, t, min(limit, e.capacity));
            if (pushed) {
                e.capacity -= pushed;
                g[e.to][e.reverse].capacity += pushed;
                return pushed;
            }
        }
        return 0;
    }

    int flow(int s, int t, int limit) {
        int result = 0;
        while (result < limit) {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            q.push(s);
            level[s] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (auto e : g[u])
                    if (e.capacity && level[e.to] == -1) {
                        level[e.to] = level[u] + 1;
                        q.push(e.to);
                    }
            }
            if (level[t] == -1)
                break;
            fill(next.begin(), next.end(), 0);
            while (result < limit) {
                int pushed = send(s, t, limit - result);
                if (!pushed)
                    break;
                result += pushed;
            }
        }
        return result;
    }
};

inline pair<int, int> graph_connectivity(int n,
                                         const vector<pair<int, int>> &edges) {
    if (n < 2)
        return {0, 0};
    int edge_cut = n - 1;
    for (int t = 1; t < n && edge_cut; t++) {
        ConnectivityFlow f(n);
        for (auto [u, v] : edges) {
            f.add(u, v, 1);
            f.add(v, u, 1);
        }
        edge_cut = f.flow(0, t, edge_cut);
    }
    if (!edge_cut)
        return {0, 0};

    vector adjacent(n, vector<bool>(n));
    for (auto [u, v] : edges)
        adjacent[u][v] = adjacent[v][u] = true;
    int vertex_cut = edge_cut;
    for (int s = 0; s < n; s++)
        for (int t = s + 1; t < n; t++) {
            if (adjacent[s][t])
                continue;
            ConnectivityFlow f(2 * n);
            for (int u = 0; u < n; u++)
                f.add(u, n + u, 1);
            for (auto [u, v] : edges) {
                f.add(n + u, v, n);
                f.add(n + v, u, n);
            }
            vertex_cut = f.flow(n + s, t, vertex_cut);
        }
    return {vertex_cut, edge_cut};
}
