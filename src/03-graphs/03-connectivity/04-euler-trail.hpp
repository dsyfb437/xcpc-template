/**
 * @brief 欧拉路（Hierholzer）
 * @complexity O(n + m)
 * @index 点 1-based，边编号 0-based
 *
 * 用途：求一条欧拉路（每条边恰好经过一次）。
 * 无向图 g[u] 存 {邻点, 边编号}，每条边加两次；有向图 g[u] 存出边 {v, 边编号}。
 * 返回 {ok, 点序列 vs, 边序列 es}；ok = false 表示不存在。
 * 显式栈模拟走边，退栈时记录点和入边，再反转得到答案。
 * 自环、平行边都按边编号处理。没有顶点时返回成功和空路径。
 *
 * 例：
 *   g[u].push_back({v, id}); g[v].push_back({u, id});
 *   auto res = euler_trail_undirected(g, m);
 *   if (!res.ok) 无解；否则 res.vs 是点序列，res.es 是边序列。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct EulerTrail {
    bool ok = false;
    vector<int> vs, es;
};

// 无向图欧拉路；m 是边数。
inline EulerTrail euler_trail_undirected(const vector<vector<pair<int, int>>> &g,
                                         int m) {
    int n = (int)g.size() - 1;
    EulerTrail res;
    if (n == 0) return {true, {}, {}};
    vector<int> deg(n + 1);
    int odd = 0, start = 0;
    for (int u = 1; u <= n; u++) {
        deg[u] = (int)g[u].size();
        if (deg[u] % 2) {
            odd++;
            start = u;
        }
        if (!start && deg[u])
            start = u;
    }
    if (odd != 0 && odd != 2)
        return res;
    if (!start)
        start = 1;

    // 所有有边的点必须和 start 连通
    vector<char> vis(n + 1, 0);
    vector<int> st{start};
    vis[start] = 1;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (auto [v, eid] : g[u])
            if (!vis[v]) {
                vis[v] = 1;
                st.push_back(v);
            }
    }
    for (int u = 1; u <= n; u++)
        if (deg[u] && !vis[u])
            return res;

    auto h = g;
    vector<char> used(m, 0);
    vector<int> circuit, edge_path;
    vector<pair<int, int>> frames{{start, -1}};
    while (!frames.empty()) {
        auto [u, incoming] = frames.back();
        while (!h[u].empty() && used[h[u].back().second]) h[u].pop_back();
        if (h[u].empty()) {
            circuit.push_back(u);
            if (incoming != -1) edge_path.push_back(incoming);
            frames.pop_back();
        } else {
            auto [v, id] = h[u].back();
            h[u].pop_back();
            used[id] = 1;
            frames.emplace_back(v, id);
        }
    }
    reverse(circuit.begin(), circuit.end());
    reverse(edge_path.begin(), edge_path.end());
    if ((int)circuit.size() != m + 1)
        return res;
    res.ok = true;
    res.vs = circuit;
    res.es = edge_path;
    return res;
}

// 有向图欧拉路；g[u] 是出边。
inline EulerTrail euler_trail_directed(const vector<vector<pair<int, int>>> &g,
                                       int m) {
    int n = (int)g.size() - 1;
    EulerTrail res;
    if (n == 0) return {true, {}, {}};
    vector<int> in(n + 1), out(n + 1);
    for (int u = 1; u <= n; u++)
        for (auto [v, eid] : g[u]) {
            out[u]++;
            in[v]++;
        }

    int start = 0, plus = 0, minus = 0;
    for (int u = 1; u <= n; u++) {
        int d = out[u] - in[u];
        if (d == 1)
            start = u, plus++;
        else if (d == -1)
            minus++;
        else if (d != 0)
            return res;
        if (!start && out[u])
            start = u;
    }
    bool okdeg = (plus == 0 && minus == 0) || (plus == 1 && minus == 1);
    if (!okdeg)
        return res;
    if (!start)
        start = 1;

    // 忽略方向，检查弱连通
    vector<vector<int>> ug(n + 1);
    for (int u = 1; u <= n; u++)
        for (auto [v, eid] : g[u]) {
            ug[u].push_back(v);
            ug[v].push_back(u);
        }
    vector<char> vis(n + 1, 0);
    vector<int> st{start};
    vis[start] = 1;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (int v : ug[u])
            if (!vis[v]) {
                vis[v] = 1;
                st.push_back(v);
            }
    }
    for (int u = 1; u <= n; u++)
        if ((in[u] || out[u]) && !vis[u])
            return res;

    auto h = g;
    vector<char> used(m, 0);
    vector<int> circuit, edge_path;
    vector<pair<int, int>> frames{{start, -1}};
    while (!frames.empty()) {
        auto [u, incoming] = frames.back();
        while (!h[u].empty() && used[h[u].back().second]) h[u].pop_back();
        if (h[u].empty()) {
            circuit.push_back(u);
            if (incoming != -1) edge_path.push_back(incoming);
            frames.pop_back();
        } else {
            auto [v, id] = h[u].back();
            h[u].pop_back();
            used[id] = 1;
            frames.emplace_back(v, id);
        }
    }
    reverse(circuit.begin(), circuit.end());
    reverse(edge_path.begin(), edge_path.end());
    if ((int)circuit.size() != m + 1)
        return res;
    res.ok = true;
    res.vs = circuit;
    res.es = edge_path;
    return res;
}
