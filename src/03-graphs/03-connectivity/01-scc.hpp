/**
 * @brief 强连通分量（Tarjan）
 * @complexity O(n + m)
 * @index 点 1-based
 *
 * 用途：求有向图的 SCC，`comp[i]` 是 i 的分量编号（0..cnt-1），编号是反拓扑序。
 * 用法：`TarjanSCC scc(g);` 之后 `scc.comp[i]`、`scc.cnt`。
 * 判 2-SAT 时看 x 和 x+n 是否同分量；赋值用 comp[x] < comp[x+n]。
 * DFS 用显式栈记录走到了哪条出边。
 *
 * 例：
 *   TarjanSCC scc(g);
 *   if (scc.comp[u] == scc.comp[v]) 同一个分量
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct TarjanSCC {
    int n, cnt = 0, timer = 0;
    vector<int> comp, dfn, low, stk;
    vector<char> in_stk;

    explicit TarjanSCC(const vector<vector<int>> &g)
        : n((int)g.size() - 1), comp(n + 1, -1), dfn(n + 1), low(n + 1),
          in_stk(n + 1, 0) {
        for (int u = 1; u <= n; u++)
            if (!dfn[u])
                dfs(u, g);
    }

    void dfs(int u, const vector<vector<int>> &g) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in_stk[u] = 1;
        vector<pair<int, int>> frames{{u, 0}};
        while (!frames.empty()) {
            auto &[x, next] = frames.back();
            if (next < (int)g[x].size()) {
                int v = g[x][next++];
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++timer;
                    stk.push_back(v);
                    in_stk[v] = 1;
                    frames.emplace_back(v, 0);
                } else if (in_stk[v]) {
                    low[x] = min(low[x], dfn[v]);
                }
                continue;
            }
            int finished = x;
            frames.pop_back();
            if (!frames.empty()) {
                int parent = frames.back().first;
                low[parent] = min(low[parent], low[finished]);
            }
            if (low[finished] != dfn[finished]) continue;
            while (true) {
                int v = stk.back();
                stk.pop_back();
                in_stk[v] = 0;
                comp[v] = cnt;
                if (v == finished) break;
            }
            cnt++;
        }
    }

};
