/**
 * @brief 树分块（王室联邦）
 * @complexity O(n)
 * @index 点和块 1-based
 * @idea DFS 中暂存没分组的点。一个点收够 B 个待分点就取成一块，以这个点为块的代表。
 * @usage auto [block, capital] = tree_blocks(g, B, root); capital[block[u]] 是 u 所在块的代表。
 * @pitfall B 至少为 1，g 是非空树。代表可以不在本块，块内点到代表的路径在本块与代表内。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<vector<int>, vector<int>> tree_blocks(const vector<vector<int>> &g,
                                                  int B, int root = 1) {
    vector<int> block(g.size()), capital{0}, pending;
    auto dfs = [&](auto &&self, int u, int parent) -> void {
        int base = (int)pending.size();
        for (int v : g[u]) {
            if (v == parent)
                continue;
            self(self, v, u);
            if ((int)pending.size() - base < B)
                continue;
            int id = (int)capital.size();
            capital.push_back(u);
            while ((int)pending.size() > base) {
                block[pending.back()] = id;
                pending.pop_back();
            }
        }
        pending.push_back(u);
    };
    dfs(dfs, root, 0);
    if (capital.size() == 1)
        capital.push_back(root);
    for (int u : pending)
        block[u] = (int)capital.size() - 1;
    return {move(block), move(capital)};
}
