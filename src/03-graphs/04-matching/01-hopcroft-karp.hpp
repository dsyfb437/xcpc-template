/**
 * @brief 二分图最大匹配（Hopcroft-Karp）
 * @complexity O(E sqrt V)
 * @index 左右部 1-based
 *
 * 左部 1..n，右部 1..m；g[u] 存左点 u 能连的右点。
 * 用法：`HopcroftKarp hk(n, m); hk.add_edge(u, v);`
 *       `int k = hk.max_matching();` 之后 `hk.match_l[u]`、`hk.match_r[v]`。
 * 按层 BFS + 沿层 DFS 增广，一次找多条最短路。
 * 可在已有匹配上继续加边，max_matching 每次都返回当前匹配总数。
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
    vector<int> match_l, match_r, dist, next;

    HopcroftKarp(int n, int m)
        : n(n), m(m), g(n + 1), match_l(n + 1, 0), match_r(m + 1, 0), dist(n + 1),
          next(n + 1) {}

    void add_edge(int u, int v) { g[u].push_back(v); }

    bool bfs() {
        queue<int> q;
        fill(dist.begin(), dist.end(), -1);
        fill(next.begin(), next.end(), 0);
        for (int u = 1; u <= n; u++) {
            if (match_l[u] == 0) {
                dist[u] = 0;
                q.push(u);
            }
        }
        // 0 表示未匹配的右点，dist[0] 记录最短增广路的层数。
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[0] != -1 && dist[u] >= dist[0])
                continue;
            for (int v : g[u]) {
                int w = match_r[v];
                if (dist[w] != -1)
                    continue;
                dist[w] = dist[u] + 1;
                if (w != 0)
                    q.push(w);
            }
        }
        return dist[0] != -1;
    }

    bool dfs(int u) {
        vector<pair<int, int>> path;
        while (true) {
            bool descended = false;
            while (next[u] < (int)g[u].size()) {
                int v = g[u][next[u]++], w = match_r[v];
                if (dist[w] != dist[u] + 1)
                    continue;
                path.emplace_back(u, v);
                if (w == 0) {
                    for (auto [left, right] : path) {
                        match_l[left] = right;
                        match_r[right] = left;
                    }
                    return true;
                }
                u = w;
                descended = true;
                break;
            }
            if (descended)
                continue;
            dist[u] = -1;
            if (path.empty())
                return false;
            u = path.back().first;
            path.pop_back();
        }
    }

    int max_matching() {
        int res = 0;
        for (int u = 1; u <= n; u++)
            res += match_l[u] != 0;
        while (bfs())
            for (int u = 1; u <= n; u++)
                if (match_l[u] == 0 && dfs(u))
                    res++;
        return res;
    }
};
