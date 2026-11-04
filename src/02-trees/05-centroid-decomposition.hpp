/**
 * @brief 点分治（重心分解）
 * @complexity O(n log n)
 * @index 点 1-based
 *
 * @idea 处理树上路径统计（距离、计数等），每层取重心再递归。
 * @usage `CentroidDecomposition cd(g);`
 *        `cd.build(1, [&](int c) { ... });`
 * 回调在删 c 之前执行：对每个未删子树 DFS，统计经过 c 的路径。
 * cpar/cdep 是重心树父亲/深度；dead 是上层已删点。
 * @pitfall g 必须是一棵连通树；n≥1；g 要活过 cd；只 build 一次。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct CentroidDecomposition {
    int n;
    const vector<vector<int>> &g;
    vector<int> sz, cpar, cdep, tmp_par;
    vector<char> dead;

    explicit CentroidDecomposition(const vector<vector<int>> &g)
        : n((int)g.size() - 1), g(g), sz(n + 1), cpar(n + 1), cdep(n + 1),
          tmp_par(n + 1), dead(n + 1, 0) {}

    int centroid(int entry) {
        vector<int> order{entry};
        tmp_par[entry] = 0;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            sz[u] = 1;
            for (int v : g[u]) {
                if (v == tmp_par[u] || dead[v]) continue;
                tmp_par[v] = u;
                order.push_back(v);
            }
        }
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int u = order[i];
            sz[tmp_par[u]] += sz[u];
        }

        int total = (int)order.size();
        for (int u : order) {
            int mx = total - sz[u];
            for (int v : g[u])
                if (!dead[v] && tmp_par[v] == u)
                    mx = max(mx, sz[v]);
            if (mx <= total / 2)
                return u;
        }
        return entry;
    }

    template <class F>
    int build(int entry, F &&process, int parent = 0, int depth = 0) {
        int c = centroid(entry);
        cpar[c] = parent; // 父子都在进函数时定好，process 里就能用
        cdep[c] = depth;
        process(c);
        dead[c] = 1;
        for (int v : g[c])
            if (!dead[v])
                build(v, process, c, depth + 1);
        return c;
    }
};
