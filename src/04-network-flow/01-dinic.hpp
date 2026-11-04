/**
 * @brief 最大流（Dinic）
 * @complexity O(n^2 m)，单位容量图 O(m sqrt m)
 * @index 点 1-based
 * @idea BFS 分层，沿层次图增广；it 记录当前弧，显式栈支持深链。
 * @usage Dinic f(n); f.add_edge(u, v, cap); f.max_flow(s, t) 返回本次新增流量。
 * @pitfall s、t 必须不同，容量非负，最大流须放进 long long。允许自环、重边；调用后保留残量图。
 */
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
        int id = (int)g[u].size();
        int back = (int)g[v].size() + (u == v);
        g[u].push_back({v, back, cap});
        g[v].push_back({u, id, 0});
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

    ll dfs(int s, int t, ll limit) {
        // 沿层次图找一条增广路，用显式栈避免深链爆栈。
        vector<pair<int, ll>> path{{s, limit}};
        while (!path.empty()) {
            auto [u, cap] = path.back();
            if (u == t) {
                for (int j = 0; j + 1 < (int)path.size(); j++) {
                    int v = path[j].first;
                    auto &e = g[v][it[v]];
                    e.cap -= cap;
                    g[e.to][e.rev].cap += cap;
                }
                return cap;
            }
            int &i = it[u];
            while (i < (int)g[u].size() &&
                   (!g[u][i].cap || level[g[u][i].to] != level[u] + 1))
                i++;
            if (i < (int)g[u].size()) {
                const auto &e = g[u][i];
                path.emplace_back(e.to, min(cap, e.cap));
            } else {
                level[u] = -1;
                path.pop_back();
                if (!path.empty())
                    it[path.back().first]++;
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        assert(s != t);
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (ll f = dfs(s, t, LLONG_MAX))
                flow += f;
        }
        return flow;
    }
};
