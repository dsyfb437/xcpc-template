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
        if (!tin[u])
            dfs(dfs, u, -1);
    }
    return tree;
}
