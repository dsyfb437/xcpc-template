/**
 * @brief 点双连通分量
 * @complexity O(n + m)
 * @index 点和边编号 0-based
 * @idea DFS 时把树边和返祖边压栈。low[v] >= tin[u] 时，从栈顶弹到边 u-v 得到一个点双。
 * @usage vertex_bcc(n, edges) 返回每个点双的点表。割点可出现在多个分量里。
 * @pitfall 无向图，允许重边，不含自环。桥单独成块，孤立点也单独成块。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<vector<int>> vertex_bcc(int n, const vector<pair<int, int>> &edges) {
    vector<vector<pair<int, int>>> g(n);
    for (int id = 0; id < (int)edges.size(); id++) {
        auto [u, v] = edges[id];
        g[u].emplace_back(v, id);
        g[v].emplace_back(u, id);
    }

    vector<vector<int>> blocks;
    vector<int> tin(n), low(n), stack, seen(n, -1);
    int timer = 0;
    auto dfs = [&](auto &&self, int u, int incoming) -> void {
        tin[u] = low[u] = ++timer;
        for (auto [v, id] : g[u]) {
            if (id == incoming)
                continue;
            if (!tin[v]) {
                stack.push_back(id);
                self(self, v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] < tin[u])
                    continue;

                int block_id = (int)blocks.size();
                auto &block = blocks.emplace_back();
                while (true) {
                    int edge = stack.back();
                    stack.pop_back();
                    auto [a, b] = edges[edge];
                    for (int x : {a, b})
                        if (seen[x] != block_id) {
                            seen[x] = block_id;
                            block.push_back(x);
                        }
                    if (edge == id)
                        break;
                }
            } else if (tin[v] < tin[u]) {
                stack.push_back(id);
                low[u] = min(low[u], tin[v]);
            }
        }
    };
    for (int u = 0; u < n; u++) {
        if (g[u].empty())
            blocks.push_back({u});
        else if (!tin[u])
            dfs(dfs, u, -1);
    }
    return blocks;
}
