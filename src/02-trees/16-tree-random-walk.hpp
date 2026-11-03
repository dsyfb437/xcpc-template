/**
 * @brief 树上随机游走（首次到达期望）
 * @complexity O(n)
 * @index 点 1-based
 * @idea 把目标当根。子树有 s 个点时，从子树根首次走到父亲期望用 2s - 1 步。沿祖先链累加。
 * @usage auto steps = expected_tree_walk(g, target); steps[u] 是从 u 首次到达 target 的期望步数。
 * @pitfall g 是非空无权无向树，每一步等概率选一条邻边。目标点答案为 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> expected_tree_walk(const vector<vector<int>> &g, int target) {
    vector<int> parent(g.size()), size(g.size(), 1), order{target};
    for (int i = 0; i < (int)order.size(); i++) {
        int u = order[i];
        for (int v : g[u])
            if (v != parent[u]) {
                parent[v] = u;
                order.push_back(v);
            }
    }
    for (int i = (int)order.size() - 1; i > 0; i--) {
        int u = order[i];
        size[parent[u]] += size[u];
    }
    vector<ll> steps(g.size());
    for (int i = 1; i < (int)order.size(); i++) {
        int u = order[i];
        steps[u] = steps[parent[u]] + 2LL * size[u] - 1;
    }
    return steps;
}
