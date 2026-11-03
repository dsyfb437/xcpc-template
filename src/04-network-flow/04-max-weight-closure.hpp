/**
 * @brief 最大权闭合子图
 * @complexity 一次最大流
 * @index 点 0-based
 * @idea 正权点从源点连入，负权点连到汇点。依赖 u 必选 v 就连 u 到 v，容量大于全部正权和。
 * @usage maximum_weight_closure(weight, requires_edge) 返回 {最大权和, 选中的点表}，依赖为 {u, v}。
 * @pitfall 允许空集。正权和、权值绝对值和最大流须放进 long long，正权和须小于 LLONG_MAX。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct ClosureFlow {
    struct Edge {
        int to, reverse;
        ll cap;
    };
    vector<vector<Edge>> g;
    vector<int> level, next;

    explicit ClosureFlow(int n) : g(n), level(n), next(n) {}

    int add(int u, int v, ll cap) {
        int id = (int)g[u].size();
        int back = (int)g[v].size() + (u == v);
        g[u].push_back({v, back, cap});
        g[v].push_back({u, id, 0});
        return id;
    }

    ll send(int u, int t, ll limit) {
        if (u == t)
            return limit;
        for (int &i = next[u]; i < (int)g[u].size(); i++) {
            auto &e = g[u][i];
            if (!e.cap || level[e.to] != level[u] + 1)
                continue;
            ll sent = send(e.to, t, min(limit, e.cap));
            if (sent) {
                e.cap -= sent;
                g[e.to][e.reverse].cap += sent;
                return sent;
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

inline pair<ll, vector<int>>
maximum_weight_closure(const vector<ll> &weight,
                       const vector<pair<int, int>> &requires_edge) {
    int n = (int)weight.size(), s = n, t = n + 1;
    ClosureFlow flow(n + 2);
    ll positive = 0;
    for (int u = 0; u < n; u++) {
        if (weight[u] > 0) {
            positive += weight[u];
            flow.add(s, u, weight[u]);
        } else if (weight[u] < 0) {
            flow.add(u, t, -weight[u]);
        }
    }
    for (auto [u, v] : requires_edge)
        flow.add(u, v, positive + 1);
    ll value = positive - flow.flow(s, t, positive);
    auto side = flow.reachable(s);
    vector<int> chosen;
    for (int u = 0; u < n; u++)
        if (side[u])
            chosen.push_back(u);
    return {value, move(chosen)};
}
