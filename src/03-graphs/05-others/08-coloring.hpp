/**
 * @brief 最少颜色染色（DSATUR 搜索）
 * @complexity 最坏指数级
 * @index 点和颜色 0-based
 * @idea 优先染相邻颜色种类最多的点，先试已经用过的颜色，再试一种新颜色。
 * @usage auto [k, color] = minimum_coloring(g); k 是色数，color[u] 是点 u 的颜色。
 * @pitfall 输入为无自环的无向图，只适合小图。空图返回 0 色。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<int, vector<int>> minimum_coloring(const vector<vector<int>> &g) {
    int n = (int)g.size(), best = n + 1;
    vector<int> color(n, -1), answer;
    auto dfs = [&](auto &&self, int remaining, int used) -> void {
        if (used >= best)
            return;
        if (remaining == 0) {
            best = used;
            answer = color;
            return;
        }

        int chosen = -1, highest = -1;
        vector<bool> forbidden;
        for (int u = 0; u < n; u++) {
            if (color[u] != -1)
                continue;
            vector<bool> blocked(used + 1);
            int kinds = 0;
            for (int v : g[u])
                if (color[v] != -1 && !blocked[color[v]]) {
                    blocked[color[v]] = true;
                    kinds++;
                }
            if (kinds > highest ||
                (kinds == highest && g[u].size() > g[chosen].size())) {
                chosen = u;
                highest = kinds;
                forbidden = move(blocked);
            }
        }

        for (int c = 0; c <= used; c++) {
            if (forbidden[c])
                continue;
            color[chosen] = c;
            self(self, remaining - 1, used + (c == used));
        }
        color[chosen] = -1;
    };
    dfs(dfs, n, 0);
    return {best, move(answer)};
}
