/**
 * @brief 插头 DP（网格哈密顿回路计数）
 * @complexity 期望 O(RC^2 × 有效轮廓状态数)，C 是较短边
 * @index 网格 0-based
 * @idea 逐格扫描，用括号配对记录轮廓线上同一条路径的两个端点。合并路径时调整配对，只有最后一个可走格能闭环。
 * @usage grid_hamilton_cycles(grid,mod) 返回恰好走遍所有可走格的无向回路数。
 * @pitfall 井号为障碍，其余格可走。允许转置，较短边至多 14。mod 为正 int，无可走格返回 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int grid_hamilton_cycles(vector<string> grid, int mod) {
    if (grid.empty() || grid[0].empty())
        return 0;
    int rows = (int)grid.size(), cols = (int)grid[0].size();
    if (rows < cols) {
        vector<string> rotated(cols, string(rows, '#'));
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                rotated[j][i] = grid[i][j];
        grid.swap(rotated);
        swap(rows, cols);
    }
    assert(cols <= 14);

    int last = -1;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (grid[i][j] != '#')
                last = i * cols + j;
    if (last == -1)
        return 0;

    using Mask = unsigned;
    unordered_map<Mask, int> dp{{0, 1 % mod}}, next;
    int answer = 0;
    auto get = [](Mask mask, int i) { return (int)(mask >> (2 * i) & 3); };
    auto put = [](Mask mask, int i, int value) {
        return (mask & ~(3U << (2 * i))) | (Mask(value) << (2 * i));
    };
    auto add = [&](Mask mask, int count) {
        int &x = next[mask];
        x = (x + (ll)count) % mod;
    };

    for (int i = 0; i < rows; i++) {
        next.clear();
        for (auto [mask, count] : dp)
            add(mask << 2, count);
        dp.swap(next);
        for (int j = 0; j < cols; j++) {
            next.clear();
            bool down = i + 1 < rows && grid[i + 1][j] != '#';
            bool right = j + 1 < cols && grid[i][j + 1] != '#';
            for (auto [mask, count] : dp) {
                int a = get(mask, j), b = get(mask, j + 1);
                Mask rest = put(put(mask, j, 0), j + 1, 0);
                if (grid[i][j] == '#') {
                    if (!a && !b)
                        add(rest, count);
                } else if (!a && !b) {
                    if (down && right)
                        add(put(put(rest, j, 1), j + 1, 2), count);
                } else if (!a || !b) {
                    if (down)
                        add(put(rest, j, a + b), count);
                    if (right)
                        add(put(rest, j + 1, a + b), count);
                } else if (a == 1 && b == 2) {
                    if (!rest && i * cols + j == last)
                        answer = (answer + (ll)count) % mod;
                } else if (a == 2 && b == 1) {
                    add(rest, count);
                } else if (a == 1) {
                    int depth = 1;
                    for (int k = j + 2; k <= cols; k++) {
                        int p = get(mask, k);
                        depth += (p == 1) - (p == 2);
                        if (!depth) {
                            add(put(rest, k, 1), count);
                            break;
                        }
                    }
                } else {
                    int depth = 1;
                    for (int k = j - 1; k >= 0; k--) {
                        int p = get(mask, k);
                        depth += (p == 2) - (p == 1);
                        if (!depth) {
                            add(put(rest, k, 2), count);
                            break;
                        }
                    }
                }
            }
            dp.swap(next);
        }
    }
    return answer;
}
