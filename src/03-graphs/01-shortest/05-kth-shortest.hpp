/**
 * @brief k 短路（A*）
 * @complexity O((n + m) log n + km log(km + 1))
 * @index 点 1-based，k 从 1 开始
 *
 * @idea 先在反图求每个点到 t 的最短距离 h。按已走距离加 h 出堆，
 *       第 k 次取到 t 就得到答案，每个点最多展开 k 次。
 * @usage kth_shortest(g, s, t, k)，g[u] 存 {v, w}。不足 k 条返回 -1。
 * @pitfall 边权非负。允许重复经过点和边，重边分开计。
 *          s 等于 t 时把空路径算第一条。距离及估价须小于 2^62。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll kth_shortest(const vector<vector<pair<int, ll>>> &g, int s, int t,
                       int k) {
    if (k <= 0)
        return -1;

    constexpr ll inf = 1LL << 62;
    vector<vector<pair<int, ll>>> rev(g.size());
    for (int u = 1; u < (int)g.size(); u++)
        for (auto [v, w] : g[u])
            rev[v].emplace_back(u, w);

    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    vector<ll> h(g.size(), inf);
    h[t] = 0;
    pq.emplace(0, t);
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != h[u])
            continue;
        for (auto [v, w] : rev[u]) {
            if (d + w >= h[v])
                continue;
            h[v] = d + w;
            pq.emplace(h[v], v);
        }
    }
    if (h[s] == inf)
        return -1;

    using Estimate = tuple<ll, ll, int>;
    priority_queue<Estimate, vector<Estimate>, greater<Estimate>> candidates;
    vector<int> popped(g.size());
    candidates.emplace(h[s], 0, s);
    while (!candidates.empty()) {
        auto [estimate, d, u] = candidates.top();
        candidates.pop();
        if (popped[u] == k)
            continue;
        popped[u]++;
        if (u == t && popped[u] == k)
            return d;
        for (auto [v, w] : g[u])
            if (h[v] != inf && popped[v] < k)
                candidates.emplace(d + w + h[v], d + w, v);
    }
    return -1;
}
