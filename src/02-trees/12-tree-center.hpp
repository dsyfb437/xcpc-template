/**
 * @brief 树的中心
 * @complexity O(n)
 * @index 点 1-based
 * @idea 一层层删叶子。最后剩下的一到两个点就是到最远点距离最小的点。
 * @usage auto centers = tree_centers(g); 返回所有中心。
 * @pitfall g 是无权无向树，空树返回空数组。树的中心和按子树大小定义的重心是两回事。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> tree_centers(const vector<vector<int>> &g) {
    int n = (int)g.size() - 1;
    if (n <= 0)
        return {};
    vector<int> degree(n + 1);
    queue<int> leaves;
    for (int u = 1; u <= n; u++) {
        degree[u] = (int)g[u].size();
        if (degree[u] <= 1)
            leaves.push(u);
    }
    int remaining = n;
    while (remaining > 2) {
        int layer = (int)leaves.size();
        remaining -= layer;
        for (int i = 0; i < layer; i++) {
            int u = leaves.front();
            leaves.pop();
            for (int v : g[u])
                if (--degree[v] == 1)
                    leaves.push(v);
        }
    }
    vector<int> centers;
    while (!leaves.empty()) {
        centers.push_back(leaves.front());
        leaves.pop();
    }
    return centers;
}
