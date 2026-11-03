/**
 * @brief 带权最小直径生成树
 * @complexity O(n^3 + mn log n)
 * @index 点和边编号 0-based
 * @idea 树的中心可以在边内。Floyd 后逐边扫描距离函数的转折点，找最小的最大距离，再从该点建最短路树。
 * @usage minimum_diameter_tree(n, edges) 返回 {最小直径, 原图边编号表}，每条输入边为 {u, v, w}。
 * @pitfall 无向图，边权为非负整数。不连通返回 {-1, 空表}。最长有限路径及中间倍增值须小于 2^60。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<ll, vector<int>>
minimum_diameter_tree(int n, const vector<tuple<int, int, ll>> &edges) {
    if (n <= 1)
        return {0, {}};
    constexpr ll inf = 1LL << 60;
    vector d(n, vector<ll>(n, inf));
    for (int u = 0; u < n; u++)
        d[u][u] = 0;
    for (auto [u, v, w] : edges)
        d[u][v] = d[v][u] = min(d[u][v], w);
    for (int k = 0; k < n; k++)
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++)
                d[u][v] = min(d[u][v], d[u][k] + d[k][v]);
    for (ll value : d[0])
        if (value == inf)
            return {-1, {}};

    ll best = inf, offset = 0;
    int center_edge = -1;
    for (int id = 0; id < (int)edges.size(); id++) {
        auto [u, v, w] = edges[id];
        if (u == v)
            continue;
        vector<pair<ll, int>> switches;
        for (int x = 0; x < n; x++)
            switches.emplace_back(d[v][x] + w - d[u][x], x);
        sort(switches.begin(), switches.end());

        // 距 u 的偏移取两倍，避免半整数。
        vector<ll> suffix(n + 1, -inf);
        for (int i = n - 1; i >= 0; i--)
            suffix[i] = max(suffix[i + 1], 2 * d[u][switches[i].second]);
        ll right = -inf, low = 0;
        for (int i = 0; i <= n; i++) {
            ll high = i == n ? 2 * w : switches[i].first;
            ll x = clamp((right - suffix[i]) / 2, low, high);
            ll radius = max(suffix[i] + x, right - x);
            if (radius < best) {
                best = radius;
                center_edge = id;
                offset = x;
            }
            if (i != n) {
                right = max(right, 2 * (d[v][switches[i].second] + w));
                low = high;
            }
        }
    }

    // 把中心当作新根，原中心边拆成两条边。
    vector<vector<tuple<int, ll, int>>> g(n + 1);
    auto add = [&](int u, int v, ll w, int id) {
        g[u].emplace_back(v, w, id);
        g[v].emplace_back(u, w, id);
    };
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v, w] = edges[i];
        if (i == center_edge) {
            add(n, u, offset, -1);
            add(n, v, 2 * w - offset, -1);
        } else {
            add(u, v, 2 * w, i);
        }
    }

    vector<ll> dist(n + 1, inf);
    vector<int> parent_edge(n, -1);
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> q;
    dist[n] = 0;
    q.emplace(0, n);
    while (!q.empty()) {
        auto [du, u] = q.top();
        q.pop();
        if (du != dist[u])
            continue;
        for (auto [v, w, id] : g[u])
            if (du + w < dist[v]) {
                dist[v] = du + w;
                parent_edge[v] = id;
                q.emplace(dist[v], v);
            }
    }
    vector<int> tree;
    int root_children = 0;
    for (int id : parent_edge) {
        if (id == -1)
            root_children++;
        else
            tree.push_back(id);
    }
    if (root_children == 2)
        tree.push_back(center_edge);
    return {best, move(tree)};
}
