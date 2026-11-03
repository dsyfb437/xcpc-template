/**
 * @brief 悬线法（全 1 最大矩形）
 * @complexity O(nm) 时间，O(m) 空间
 * @index grid 每行是一个 01 字符串
 * @idea 固定底边，把连续向上的 1 数成高度。每根悬线向两边跨过不低于它的列，算出能撑开的最大矩形。
 * @usage long long area = largest_ones_rectangle(grid);
 * @pitfall 相同高度可以跨过。每行长度一致，空网格返回 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll largest_ones_rectangle(const vector<string> &grid) {
    if (grid.empty())
        return 0;
    int m = (int)grid[0].size();
    vector<int> height(m), left(m), right(m);
    ll best = 0;
    for (const auto &row : grid) {
        for (int i = 0; i < m; i++)
            height[i] = row[i] == '1' ? height[i] + 1 : 0;
        for (int i = 0; i < m; i++) {
            int j = i - 1;
            while (j >= 0 && height[j] >= height[i])
                j = left[j];
            left[i] = j;
        }
        for (int i = m - 1; i >= 0; i--) {
            int j = i + 1;
            while (j < m && height[j] >= height[i])
                j = right[j];
            right[i] = j;
            best = max(best, 1LL * height[i] * (right[i] - left[i] - 1));
        }
    }
    return best;
}
