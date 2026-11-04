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
    struct Frame { int u, incoming, next; };
    for (int root = 0; root < n; root++) {
        if (tin[root]) continue;
        tin[root] = low[root] = ++timer;
        if (g[root].empty()) blocks.push_back({root});
        vector<Frame> frames{{root, -1, 0}};
        while (!frames.empty()) {
            auto &f = frames.back();
            if (f.next < (int)g[f.u].size()) {
                auto [v, id] = g[f.u][f.next++];
                if (id == f.incoming) continue;
                if (!tin[v]) {
                    stack.push_back(id);
                    tin[v] = low[v] = ++timer;
                    frames.push_back({v, id, 0});
                } else if (tin[v] < tin[f.u]) {
                    stack.push_back(id);
                    low[f.u] = min(low[f.u], tin[v]);
                }
                continue;
            }
            int u = f.u, id = f.incoming;
            frames.pop_back();
            if (frames.empty()) continue;
            int parent = frames.back().u;
            low[parent] = min(low[parent], low[u]);
            if (low[u] < tin[parent]) continue;
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
                if (edge == id) break;
            }
        }
    }
    return blocks;
}
