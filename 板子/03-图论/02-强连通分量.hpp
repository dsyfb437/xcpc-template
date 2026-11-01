/**
 * @brief 强连通分量（Tarjan）
 * @complexity O(n + m)
 * @index 点 1-based
 *
 * 用途：求有向图的 SCC，`comp[i]` 是 i 的分量编号（0..cnt-1），编号是反拓扑序。
 * 用法：`TarjanSCC scc(g);` 之后 `scc.comp[i]`、`scc.cnt`。
 * 判 2-SAT 时看 x 和 x+n 是否同分量；赋值用 comp[x] < comp[x+n]。
 * 递归 Tarjan；深图本地测试前先 `ulimit -s unlimited`。
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
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v, g);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            while (true) {
                int v = stk.back();
                stk.pop_back();
                in_stk[v] = 0;
                comp[v] = cnt;
                if (v == u)
                    break;
            }
            cnt++;
        }
    }
};
