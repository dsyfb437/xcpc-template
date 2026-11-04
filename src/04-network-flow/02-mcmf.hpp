/**
 * @brief 最小费用最大流（SPFA 增广）
 * @complexity O(F n m)，F 是最终流量
 * @index 点 1-based
 * @idea 每次用 SPFA 找费用最小的增广路，沿路送流。
 * @usage MCMF f(n); f.add_edge(u, v, cap, cost); f.flow(s, t) 返回本次新增的 {流量, 费用}。
 * @pitfall s、t 必须不同，容量非负。允许负边，但不能有负费用环。流量和答案须放进 long long；距离和费用中间量用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MCMF {
    using Wide = __int128;
    struct Edge {
        int to, rev;
        ll cap;
        Wide cost;
    };

    int n;
    vector<vector<Edge>> g;

    explicit MCMF(int n) : n(n), g(n + 1) {}

    void add_edge(int u, int v, ll cap, ll cost) {
        int id = (int)g[u].size();
        int back = (int)g[v].size() + (u == v);
        g[u].push_back({v, back, cap, Wide(cost)});
        g[v].push_back({u, id, 0, -Wide(cost)});
    }

    // 返回 {最大流量, 最小费用}
    pair<ll, ll> flow(int s, int t) {
        assert(s != t);
        constexpr Wide INF = Wide{1} << 120;
        ll total_flow = 0;
        Wide total_cost = 0;
        vector<Wide> dist(n + 1);
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

            ll add = LLONG_MAX;
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
        return {total_flow, (ll)total_cost};
    }
};
