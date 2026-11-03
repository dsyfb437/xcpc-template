/**
 * @brief 记忆化搜索（网格最长递增路）
 * @complexity O(RC)
 * @index 网格 0-based
 * @idea 把格子当状态，只走到四邻中更大的格子，所以没有环。第一次递归时算好答案，之后直接查表。
 * @usage longest_increasing_grid_path(a) 返回最长路径的点数。
 * @pitfall 网格为矩形，空网格返回 0。递增链很长时注意递归栈。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int longest_increasing_grid_path(const vector<vector<int>> &a) {
    if (a.empty() || a[0].empty())
        return 0;
    int rows = (int)a.size(), cols = (int)a[0].size();
    vector memo(rows, vector<int>(cols));
    constexpr int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};
    auto dfs = [&](auto &&self, int x, int y) -> int {
        int &best = memo[x][y];
        if (best)
            return best;
        best = 1;
        for (int d = 0; d < 4; d++) {
            int u = x + dx[d], v = y + dy[d];
            if (0 <= u && u < rows && 0 <= v && v < cols && a[u][v] > a[x][y])
                best = max(best, 1 + self(self, u, v));
        }
        return best;
    };
    int answer = 0;
    for (int x = 0; x < rows; x++)
        for (int y = 0; y < cols; y++)
            answer = max(answer, dfs(dfs, x, y));
    return answer;
}
