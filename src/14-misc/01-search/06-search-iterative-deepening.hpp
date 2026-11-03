/**
 * @brief 迭代加深搜索（IDDFS）
 * @complexity 最坏指数级时间，O(n) 额外空间
 * @index 顶点 0-based
 * @idea 先只搜 0 步，然后搜 1 步、2 步。每轮用 DFS，当前路径上的点不重复走。
 * @usage auto path = iddfs_path(g, start, goal, max_depth);
 *   返回步数最少的路径，步数上限内无解时返回空数组。
 * @pitfall 边权都为 1。只能标记当前路径，回溯时记得取消。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> iddfs_path(const vector<vector<int>> &g, int start, int goal,
                              int max_depth) {
    vector<int> path = {start};
    vector<bool> on_path(g.size());
    on_path[start] = true;
    auto dfs = [&](auto &&self, int u, int left) -> bool {
        if (u == goal)
            return true;
        if (left == 0)
            return false;
        for (int v : g[u]) {
            if (on_path[v])
                continue;
            on_path[v] = true;
            path.push_back(v);
            if (self(self, v, left - 1))
                return true;
            path.pop_back();
            on_path[v] = false;
        }
        return false;
    };
    int limit = min(max_depth, (int)g.size() - 1);
    for (int depth = 0; depth <= limit; depth++)
        if (dfs(dfs, start, depth))
            return path;
    return {};
}
