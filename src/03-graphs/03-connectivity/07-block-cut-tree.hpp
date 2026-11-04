/**
 * @brief 圆方树
 * @complexity O(n + m)
 * @index 圆点 0 到 n - 1，之后是方点
 * @idea 每找到一个点双就建一个方点，把它连到这个点双的所有原图点。
 * @usage block_cut_tree(n, edges) 返回圆方森林的邻接表。只看原图两点间路径时，可在森林上继续做树上查询。
 * @pitfall 无向图，允许重边，不含自环。孤立圆点保持孤立，原图不连通时结果是森林。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<vector<int>> block_cut_tree(int n,
                                          const vector<pair<int, int>> &edges) {
    vector<vector<pair<int, int>>> g(n);
    for (int id = 0; id < (int)edges.size(); id++) {
        auto [u, v] = edges[id];
        g[u].emplace_back(v, id);
        g[v].emplace_back(u, id);
    }

    vector<vector<int>> tree(n);
    vector<int> tin(n), low(n), stack, seen(n, -1);
    int timer = 0;
    struct Frame { int u, incoming, next; };
    for (int root = 0; root < n; root++) {
        if (tin[root]) continue;
        tin[root] = low[root] = ++timer;
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
            int block_id = (int)tree.size();
            tree.emplace_back();
            while (true) {
                int edge = stack.back();
                stack.pop_back();
                auto [a, b] = edges[edge];
                for (int x : {a, b})
                    if (seen[x] != block_id) {
                        seen[x] = block_id;
                        tree[x].push_back(block_id);
                        tree[block_id].push_back(x);
                    }
                if (edge == id) break;
            }
        }
    }
    return tree;
}
