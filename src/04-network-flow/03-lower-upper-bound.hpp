/**
 * @brief 上下界网络流
 * @complexity 最多两次最大流
 * @index 点和边 0-based
 * @idea 先送下界流，按每个点的净流入量接超级源汇。有源汇时用 t 到 s 的附加边把问题变成环流。
 * @usage bounded_flow(n, edges) 求可行环流。bounded_flow(n, edges, s, t, maximize) 求最大或最小非负流，默认最大。返回 {流量, 每条边的流量}。
 * @pitfall 边为 {u, v, low, high} 且 0 <= low <= high。无解返回 nullopt。源汇不同，容量与流量总和须小于 LLONG_MAX。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct BoundedFlowNetwork {
    struct Edge {
        int to, reverse;
        ll cap;
    };
    vector<vector<Edge>> g;
    vector<int> level, next;

    explicit BoundedFlowNetwork(int n) : g(n), level(n), next(n) {}

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

    void remove(int u, int id) {
        auto &e = g[u][id];
        g[e.to][e.reverse].cap = 0;
        e.cap = 0;
    }
};

inline optional<pair<ll, vector<ll>>>
bounded_flow(int n, const vector<tuple<int, int, ll, ll>> &edges, int s = -1,
             int t = -1, bool maximize = true) {
    BoundedFlowNetwork network(n + 2);
    int source = n, sink = n + 1;
    vector<ll> balance(n);
    vector<int> original;
    for (auto [u, v, low, high] : edges) {
        if (low < 0 || low > high)
            return nullopt;
        original.push_back(network.add(u, v, high - low));
        balance[u] -= low;
        balance[v] += low;
    }

    int bridge = s == -1 ? -1 : network.add(t, s, LLONG_MAX);
    ll demand = 0;
    vector<pair<int, int>> extra;
    for (int u = 0; u < n; u++) {
        if (balance[u] > 0) {
            extra.emplace_back(source, network.add(source, u, balance[u]));
            demand += balance[u];
        } else if (balance[u] < 0) {
            extra.emplace_back(u, network.add(u, sink, -balance[u]));
        }
    }
    if (network.flow(source, sink, demand) != demand)
        return nullopt;

    ll value = 0;
    for (auto [u, id] : extra)
        network.remove(u, id);
    if (bridge != -1) {
        value = LLONG_MAX - network.g[t][bridge].cap;
        network.remove(t, bridge);
        if (maximize)
            value += network.flow(s, t, LLONG_MAX - value);
        else
            value -= network.flow(t, s, value);
    }
    vector<ll> answer;
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v, low, high] = edges[i];
        answer.push_back(high - network.g[u][original[i]].cap);
    }
    return pair{value, move(answer)};
}
