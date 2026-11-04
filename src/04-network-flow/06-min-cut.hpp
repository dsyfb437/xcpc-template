/**
 * @brief 源汇最小割
 * @complexity 一次最大流
 * @index 点和边 0-based
 * @idea 跑完最大流后，在残量图从 s 沿正容量边搜索。可达点属于源侧，原图从源侧到汇侧的边构成最小割。
 * @usage minimum_st_cut(n, edges, s, t) 返回 {割值, 源侧标记, 割边编号}。边为 {u, v, cap}。
 * @pitfall 输入有向图，s、t 不同，容量非负且总和小于 LLONG_MAX。无向边可拆成两个方向。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MinCutFlow {
    struct Edge {
        int to, reverse;
        ll cap;
    };
    vector<vector<Edge>> g;
    vector<int> level, next;

    explicit MinCutFlow(int n) : g(n), level(n), next(n) {}

    int add(int u, int v, ll cap) {
        int id = (int)g[u].size();
        int back = (int)g[v].size() + (u == v);
        g[u].push_back({v, back, cap});
        g[v].push_back({u, id, 0});
        return id;
    }

    ll send(int s, int t, ll limit) {
        // 沿层次图找一条增广路，用显式栈避免深链爆栈。
        vector<pair<int, ll>> path{{s, limit}};
        while (!path.empty()) {
            auto [u, cap] = path.back();
            if (u == t) {
                for (int j = 0; j + 1 < (int)path.size(); j++) {
                    int v = path[j].first;
                    auto &e = g[v][next[v]];
                    e.cap -= cap;
                    g[e.to][e.reverse].cap += cap;
                }
                return cap;
            }
            int &i = next[u];
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
                    next[path.back().first]++;
            }
        }
        return 0;
    }

    ll flow(int s, int t, ll limit = LLONG_MAX) {
        ll result = 0;
        while (result < limit) {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            q.push(s);
            level[s] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (const auto &e : g[u])
                    if (e.cap && level[e.to] == -1) {
                        level[e.to] = level[u] + 1;
                        q.push(e.to);
                    }
            }
            if (level[t] == -1)
                break;
            fill(next.begin(), next.end(), 0);
            while (result < limit) {
                ll sent = send(s, t, limit - result);
                if (!sent)
                    break;
                result += sent;
            }
        }
        return result;
    }

    vector<bool> reachable(int s) const {
        vector<bool> seen(g.size());
        vector<int> order{s};
        seen[s] = true;
        for (int i = 0; i < (int)order.size(); i++)
            for (const auto &e : g[order[i]])
                if (e.cap && !seen[e.to]) {
                    seen[e.to] = true;
                    order.push_back(e.to);
                }
        return seen;
    }
};

inline tuple<ll, vector<bool>, vector<int>>
minimum_st_cut(int n, const vector<tuple<int, int, ll>> &edges, int s, int t) {
    MinCutFlow flow(n);
    for (auto [u, v, cap] : edges)
        flow.add(u, v, cap);
    ll value = flow.flow(s, t);
    auto side = flow.reachable(s);
    vector<int> cut;
    for (int id = 0; id < (int)edges.size(); id++) {
        auto [u, v, cap] = edges[id];
        if (side[u] && !side[v])
            cut.push_back(id);
    }
    return {value, move(side), move(cut)};
}
