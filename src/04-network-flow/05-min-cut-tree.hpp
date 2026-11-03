/**
 * @brief 最小割树（Gomory–Hu）
 * @complexity n - 1 次最大流
 * @index 点 0-based
 * @idea 每次对一个点和它当前的父亲求最小割，按残量源侧调整父边，必要时交换两点的父边。
 * @usage gomory_hu(n, edges) 返回树边 {u, v, cut}。两点间最小割等于树路径上最小边权。
 * @pitfall 输入无向非负容量图，每条边为 {u, v, cap}，可有重边或不连通。总容量小于 LLONG_MAX。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct CutTreeFlow {
    struct Edge {
        int to, reverse;
        ll cap;
    };
    vector<vector<Edge>> g;
    vector<int> level, next;

    explicit CutTreeFlow(int n) : g(n), level(n), next(n) {}

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

inline vector<tuple<int, int, ll>>
gomory_hu(int n, const vector<tuple<int, int, ll>> &edges) {
    vector<int> parent(n);
    vector<ll> weight(n);
    for (int s = 1; s < n; s++) {
        int t = parent[s];
        CutTreeFlow network(n);
        for (auto [u, v, cap] : edges) {
            network.add(u, v, cap);
            network.add(v, u, cap);
        }
        ll cut = network.flow(s, t);
        auto side = network.reachable(s);
        for (int u = s + 1; u < n; u++)
            if (parent[u] == t && side[u])
                parent[u] = s;
        if (t != 0 && side[parent[t]]) {
            parent[s] = parent[t];
            parent[t] = s;
            weight[s] = weight[t];
            weight[t] = cut;
        } else {
            weight[s] = cut;
        }
    }
    vector<tuple<int, int, ll>> tree;
    for (int u = 1; u < n; u++)
        tree.emplace_back(u, parent[u], weight[u]);
    return tree;
}
