/**
 * @brief 最小树形图（朱刘算法）
 * @complexity O(nm)
 * @index 点 0-based
 * @idea 每个非根点选最便宜的入边。有环时把环缩成一个点，入边扣掉原终点已选入边的代价。
 * @usage directed_mst(n, root, edges) 返回最小总权值，每条边为 {u, v, w}。
 * @pitfall 有点不能从根到达时返回 nullopt。允许负边，缩点后的权值和总和需放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<ll> directed_mst(int n, int root,
                                 vector<tuple<int, int, ll>> edges) {
    ll answer = 0;
    while (true) {
        vector<ll> cost(n);
        vector<int> parent(n, -1);
        for (auto [u, v, w] : edges)
            if (u != v && v != root && (parent[v] == -1 || w < cost[v])) {
                parent[v] = u;
                cost[v] = w;
            }

        vector<int> component(n, -1), seen(n, -1);
        int count = 0;
        for (int u = 0; u < n; u++) {
            if (u != root && parent[u] == -1)
                return nullopt;
            answer += cost[u];
            int v = u;
            while (v != root && v != -1 && seen[v] == -1) {
                seen[v] = u;
                v = parent[v];
            }
            if (v == root || v == -1 || seen[v] != u)
                continue;
            component[v] = count;
            for (int x = parent[v]; x != v; x = parent[x])
                component[x] = count;
            count++;
        }
        if (count == 0)
            return answer;

        for (int &id : component)
            if (id == -1)
                id = count++;
        for (auto &[u, v, w] : edges) {
            if (component[u] != component[v])
                w -= cost[v];
            u = component[u];
            v = component[v];
        }
        root = component[root];
        n = count;
    }
}
