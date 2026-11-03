/**
 * @brief SPFA / 判负环
 * @complexity 最坏 O(nm)
 * @index 点 1-based
 * @idea 只让距离刚变小的点进队。记录当前距离对应的路径边数，达到 n 条说明存在可达负环。
 * @usage `auto d = spfa(g, s);` g[u] 存 {v, w}，有可达负环返回空数组。
 * @pitfall 只检查从 s 能走到的负环。不可达为 SPFA_INF，有限距离和松弛值须在其正负范围内。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline constexpr ll SPFA_INF = 1LL << 62;

inline vector<ll> spfa(const vector<vector<pair<int, ll>>> &g, int s) {
    int n = (int)g.size() - 1;
    vector<ll> d(n + 1, SPFA_INF);
    vector<int> length(n + 1);
    vector<bool> queued(n + 1);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    queued[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        queued[u] = false;
        for (auto [v, w] : g[u]) {
            if (d[u] + w >= d[v])
                continue;
            d[v] = d[u] + w;
            length[v] = length[u] + 1;
            if (length[v] >= n)
                return {};
            if (!queued[v]) {
                q.push(v);
                queued[v] = true;
            }
        }
    }
    return d;
}
