/**
 * @brief 斯坦纳树
 * @complexity O(3^k n + 2^k (n + m) log(n + m))
 * @index 点 0-based
 * @idea dp[mask][u] 表示连通这组终端和 u 的最小代价。先在 u 合并两组，再跑多源 Dijkstra。
 * @usage steiner_tree(g, terminals) 返回最小权值，g[u] 存 {v, w}。连不起来返回 -1。
 * @pitfall 无向图，边权非负且小于 2^62。终端数 k 很小，空间 O(2^k n)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll steiner_tree(const vector<vector<pair<int, ll>>> &g,
                       const vector<int> &terminals) {
    if (terminals.empty())
        return 0;

    constexpr ll inf = 1LL << 62;
    int n = (int)g.size(), states = 1 << terminals.size();
    vector dp(states, vector<ll>(n, inf));
    for (int i = 0; i < (int)terminals.size(); i++)
        dp[1 << i][terminals[i]] = 0;

    using State = pair<ll, int>;
    for (int mask = 1; mask < states; mask++) {
        auto &d = dp[mask];
        for (int a = (mask - 1) & mask; a; a = (a - 1) & mask) {
            int b = mask ^ a;
            if (a > b)
                continue;
            for (int u = 0; u < n; u++)
                if (dp[a][u] != inf && dp[b][u] != inf)
                    d[u] = min(d[u], dp[a][u] + dp[b][u]);
        }
        priority_queue<State, vector<State>, greater<State>> pq;
        for (int u = 0; u < n; u++)
            if (d[u] != inf)
                pq.emplace(d[u], u);
        while (!pq.empty()) {
            auto [du, u] = pq.top();
            pq.pop();
            if (du != d[u])
                continue;
            for (auto [v, w] : g[u])
                if (du + w < d[v]) {
                    d[v] = du + w;
                    pq.emplace(d[v], v);
                }
        }
    }
    ll answer = *min_element(dp.back().begin(), dp.back().end());
    return answer == inf ? -1 : answer;
}
