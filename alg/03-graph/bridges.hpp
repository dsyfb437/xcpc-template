/**
 * @brief 桥与割点（Tarjan）
 * @complexity O(n + m)
 * @index 点 1-based，边编号 0-based
 *
 * 用途：无向图的桥（删掉后连通块变多）和割点。
 * 用法：g 是 1-indexed 邻接表，元素为 {邻点, 边编号}；
 *       `Bridges br(g, m);` 之后 `br.bridge[e]`、`br.cut[v]`。
 * 递归 Tarjan；深图本地测试前先 `ulimit -s unlimited`。
 * 平行边靠边编号区分，不会被误判成桥。
 *
 * 例：
 *   g[u].push_back({v, id}); g[v].push_back({u, id});
 *   Bridges br(g, m);
 *   if (br.bridge[id]) ...;  if (br.cut[v]) ...
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Bridges {
    int n, m;
    vector<int> dfn, low;
    vector<char> bridge, cut;

    Bridges(const vector<vector<pair<int, int>>> &g, int m)
        : n((int)g.size() - 1), m(m), dfn(n + 1), low(n + 1), bridge(m, 0),
          cut(n + 1, 0) {
        int timer = 0;
        for (int root = 1; root <= n; root++)
            if (!dfn[root])
                dfs(root, -1, g, timer);
    }

    void dfs(int u, int pe, const vector<vector<pair<int, int>>> &g, int &timer) {
        dfn[u] = low[u] = ++timer;
        int child = 0;
        for (auto [v, eid] : g[u]) {
            if (eid == pe)
                continue;
            if (!dfn[v]) {
                child++;
                dfs(v, eid, g, timer);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u])
                    bridge[eid] = 1;
                if (low[v] >= dfn[u])
                    cut[u] = 1;
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (pe == -1)
            cut[u] = child >= 2;
    }
};
