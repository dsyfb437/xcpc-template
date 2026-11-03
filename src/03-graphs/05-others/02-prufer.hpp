/**
 * @brief Prüfer 序列
 * @complexity O(n log n)
 * @index 点 0-based
 * @idea 编码时反复删编号最小的叶子并记下它的邻点。解码时用出现次数恢复剩余度数。
 * @usage prufer_encode(g) 生成序列，prufer_decode(code) 返回边表。
 * @pitfall 编码输入为至少两个点的无向树，解码的点数是序列长度加二。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> prufer_encode(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<int> degree(n), code;
    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int u = 0; u < n; u++) {
        degree[u] = (int)g[u].size();
        if (degree[u] == 1)
            leaves.push(u);
    }
    for (int i = 0; i < n - 2; i++) {
        int u = leaves.top();
        leaves.pop();
        degree[u] = 0;
        for (int v : g[u])
            if (degree[v] > 0) {
                code.push_back(v);
                if (--degree[v] == 1)
                    leaves.push(v);
                break;
            }
    }
    return code;
}

inline vector<pair<int, int>> prufer_decode(const vector<int> &code) {
    int n = (int)code.size() + 2;
    vector<int> degree(n, 1);
    for (int u : code)
        degree[u]++;
    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int u = 0; u < n; u++)
        if (degree[u] == 1)
            leaves.push(u);

    vector<pair<int, int>> edges;
    for (int u : code) {
        int leaf = leaves.top();
        leaves.pop();
        edges.emplace_back(leaf, u);
        if (--degree[u] == 1)
            leaves.push(u);
    }
    int u = leaves.top();
    leaves.pop();
    edges.emplace_back(u, leaves.top());
    return edges;
}
