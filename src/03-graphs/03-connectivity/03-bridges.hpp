/**
 * @brief 桥与割点（Tarjan）
 * @complexity O(n + m)
 * @index 点 1-based，边编号 0-based
 *
 * 用途：无向图的桥（删掉后连通块变多）和割点。
 * 用法：g 是 1-indexed 邻接表，元素为 {邻点, 边编号}；
 *       `Bridges br(g, m);` 之后 `br.bridge[e]`、`br.cut[v]`。
 * DFS 用显式栈，子树处理完再更新父亲的 low。
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
        struct Frame { int u, edge, next, children; };
        vector<Frame> stack{{u, pe, 0, 0}};
        dfn[u] = low[u] = ++timer;
        while (!stack.empty()) {
            auto &f = stack.back();
            if (f.next < (int)g[f.u].size()) {
                auto [v, edge] = g[f.u][f.next++];
                if (edge == f.edge) continue;
                if (!dfn[v]) {
                    f.children++;
                    dfn[v] = low[v] = ++timer;
                    stack.push_back({v, edge, 0, 0});
                } else {
                    low[f.u] = min(low[f.u], dfn[v]);
                }
                continue;
            }
            auto done = f;
            stack.pop_back();
            if (done.edge == -1) cut[done.u] = done.children >= 2;
            if (stack.empty()) continue;
            int parent = stack.back().u;
            low[parent] = min(low[parent], low[done.u]);
            if (low[done.u] > dfn[parent]) bridge[done.edge] = 1;
            if (low[done.u] >= dfn[parent]) cut[parent] = 1;
        }
    }

};
