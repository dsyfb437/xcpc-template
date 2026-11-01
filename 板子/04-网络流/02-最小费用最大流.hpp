/**
 * @brief 最小费用最大流（SPFA 增广）
 * @complexity O(F · n · m)，F 是最终流量
 * @index 点 1-based
 *
 * 每次沿「单位费用最短路」增广一条，适合图不大、费用非负的题。
 * 例：
 *   MCMF mcmf(n);
 *   mcmf.add_edge(u, v, cap, cost);
 *   auto [flow, cost] = mcmf.flow(s, t);
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MCMF {
    struct Edge {
        int to, rev;
        ll cap, cost;
    };

    int n;
    vector<vector<Edge>> g;

    explicit MCMF(int n) : n(n), g(n + 1) {}

    void add_edge(int u, int v, ll cap, ll cost) {
        g[u].push_back({v, (int)g[v].size(), cap, cost});
        g[v].push_back({u, (int)g[u].size() - 1, 0, -cost});
    }

    // 返回 {最大流量, 最小费用}
    pair<ll, ll> flow(int s, int t) {
        const ll INF = LLONG_MAX / 4;
        ll total_flow = 0, total_cost = 0;
        vector<ll> dist(n + 1);
        vector<int> inq(n + 1), pv(n + 1), pe(n + 1);
        while (true) {
            fill(dist.begin(), dist.end(), INF);
            fill(inq.begin(), inq.end(), 0);
            queue<int> q;
            dist[s] = 0;
            q.push(s);
            inq[s] = 1;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inq[u] = 0;
                for (int i = 0; i < (int)g[u].size(); i++) {
                    Edge &e = g[u][i];
                    if (e.cap > 0 && dist[e.to] > dist[u] + e.cost) {
                        dist[e.to] = dist[u] + e.cost;
                        pv[e.to] = u;
                        pe[e.to] = i;
                        if (!inq[e.to]) {
                            inq[e.to] = 1;
                            q.push(e.to);
                        }
                    }
                }
            }
            if (dist[t] == INF)
                break;

            ll add = INF;
            for (int v = t; v != s; v = pv[v])
                add = min(add, g[pv[v]][pe[v]].cap);
            for (int v = t; v != s; v = pv[v]) {
                Edge &e = g[pv[v]][pe[v]];
                e.cap -= add;
                g[v][e.rev].cap += add;
                total_cost += add * e.cost;
            }
            total_flow += add;
        }
        return {total_flow, total_cost};
    }
};
