/**
 * @brief 拓扑排序（DFS）
 * @complexity O(n + m)
 * @index 点 1-based
 *
 * 用途：有向无环图求拓扑序；有环时返回空 vector。
 * 用法：`auto order = topo_sort(g);`，order 是 1-indexed 的点序。
 * 递归 DFS 后序反转；本地深链测试前先 `ulimit -s unlimited`。
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
    bool cycle = false;
    auto dfs = [&](auto &&self, int u) -> void {
        state[u] = 1;
        for (int v : g[u]) {
            if (state[v] == 0)
                self(self, v);
            else if (state[v] == 1)
                cycle = true;
        }
        state[u] = 2;
        order.push_back(u);
    };
    for (int u = 1; u <= n; u++)
        if (!state[u])
            dfs(dfs, u);
    if (cycle)
        return {};
    reverse(order.begin(), order.end());
    return order;
}
