/**
 * @brief 最大流（Dinic）
 * @complexity O(n^2 m)，单位容量图 O(m sqrt m)
 * @index 点 1-based
 *
 * 点从 1 开始。当前弧优化写在里面，别删 it[]。
 * 要跑费用流、上下界流的话在这个结构上改。
 *
 * 例：
 *   Dinic dinic(n);
 *   dinic.add_edge(u, v, cap);
 *   ll f = dinic.max_flow(s, t); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    explicit Dinic(int n) : n(n), g(n + 1), level(n + 1), it(n + 1) {}

    void add_edge(int u, int v, ll cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto &e : g[u])
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
        }
        return level[t] >= 0;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t)
            return f;
        for (int &i = it[u]; i < (int)g[u].size(); i++) {
            Edge &e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                ll ret = dfs(e.to, t, min(f, e.cap));
                if (ret > 0) {
                    e.cap -= ret;
                    g[e.to][e.rev].cap += ret;
                    return ret;
                }
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (ll f = dfs(s, t, LLONG_MAX))
                flow += f;
        }
        return flow;
    }
};
