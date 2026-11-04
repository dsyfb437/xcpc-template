/**
 * @brief 拓扑排序（DFS）
 * @complexity O(n + m)
 * @index 点 1-based
 *
 * 用途：有向无环图求拓扑序；有环时返回空 vector。
 * 用法：`auto order = topo_sort(g);`，order 是 1-indexed 的点序。
 * 显式栈模拟 DFS，完成时记点，最后反转后序。
 *
 * 例：
 *   vector<int> order = topo_sort(g);  // 有环返回空
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

inline vector<int> topo_sort(const vector<vector<int>> &g) {
    int n = (int)g.size() - 1;
    vector<int> state(n + 1, 0), order; // 0 未访问，1 在栈上，2 已完成
    for (int root = 1; root <= n; root++) {
        if (state[root]) continue;
        vector<pair<int, int>> stack{{root, 0}};
        state[root] = 1;
        while (!stack.empty()) {
            auto &[u, next] = stack.back();
            if (next < (int)g[u].size()) {
                int v = g[u][next++];
                if (state[v] == 1) return {};
                if (state[v] == 0) {
                    state[v] = 1;
                    stack.emplace_back(v, 0);
                }
            } else {
                state[u] = 2;
                order.push_back(u);
                stack.pop_back();
            }
        }
    }
    reverse(order.begin(), order.end());
    return order;
}
