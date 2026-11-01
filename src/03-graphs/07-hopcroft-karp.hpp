/**
 * @brief 二分图最大匹配（Hopcroft-Karp）
 * @complexity O(E sqrt V)
 * @index 左右部 1-based
 *
 * 左部 1..n，右部 1..m；g[u] 存左点 u 能连的右点。
 * 用法：`HopcroftKarp hk(n, m); hk.add_edge(u, v);`
 *       `int k = hk.max_matching();` 之后 `hk.match_l[u]`、`hk.match_r[v]`。
 * 按层 BFS + 沿层 DFS 增广，一次找多条最短路。
 *
 * 例：
 *   HopcroftKarp hk(L, R);
 *   for (...) hk.add_edge(u, v);
 *   int k = hk.max_matching(); */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct HopcroftKarp {
    int n, m;
    vector<vector<int>> g;
    vector<int> match_l, match_r, dist;

    HopcroftKarp(int n, int m)
        : n(n), m(m), g(n + 1), match_l(n + 1, 0), match_r(m + 1, 0), dist(n + 1) {}

    void add_edge(int u, int v) { g[u].push_back(v); }

    bool bfs() {
        queue<int> q;
        bool found = false;
        for (int u = 1; u <= n; u++) {
            if (match_l[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                int w = match_r[v];
                if (w == 0) {
                    found = true;
                } else if (dist[w] < 0) {
                    dist[w] = dist[u] + 1;
                    q.push(w);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            int w = match_r[v];
            if (w == 0 || (dist[w] == dist[u] + 1 && dfs(w))) {
                match_l[u] = v;
                match_r[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int max_matching() {
        int res = 0;
        while (bfs())
            for (int u = 1; u <= n; u++)
                if (match_l[u] == 0 && dfs(u))
                    res++;
        return res;
    }
};
