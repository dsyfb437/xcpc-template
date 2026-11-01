/**
 * @brief 树上启发式合并（DSU on tree）
 * @complexity O(n log n)
 * @index 点 1-based，pos 0-based
 *
 * 用途：离线回答子树统计（众数、颜色和、出现次数等）。
 * 做法：保留重儿子的数据，轻儿子暴力加进来、答完再撤销。
 * 用法：`DSUOnTree dsu(g);`
 *       `dsu.solve(1, [&](int u, int delta) { ... }, [&](int u) { ans[u] = ...; });`
 * 递归实现；本地深链测试前先 `ulimit -s unlimited`。
 *
 * 例：
 *   DSUOnTree dsu(g);
 *   dsu.solve(1, [&](int u, int d) { cnt[col[u]] += d; },
 *                [&](int u) { ans[u] = ...; });
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;

struct DSUOnTree {
    int n, cur = 0;
    const vector<vector<int>> &g;
    vector<int> par, sz, heavy, pos, rev;

    explicit DSUOnTree(const vector<vector<int>> &g)
        : n((int)g.size() - 1), g(g), par(n + 1), sz(n + 1, 1), heavy(n + 1, 0),
          pos(n + 1), rev(n) {
        dfs1(1, 0);
        dfs2(1);
    }

    void dfs1(int u, int p) {
        par[u] = p;
        sz[u] = 1;
        for (int v : g[u])
            if (v != p) {
                dfs1(v, u);
                sz[u] += sz[v];
                if (sz[v] > sz[heavy[u]])
                    heavy[u] = v;
            }
    }

    void dfs2(int u) {
        pos[u] = cur;
        rev[cur++] = u;
        if (heavy[u])
            dfs2(heavy[u]);
        for (int v : g[u])
            if (v != par[u] && v != heavy[u])
                dfs2(v);
    }

    template <class Add> void add_subtree(int u, int delta, Add &add) const {
        for (int i = pos[u]; i < pos[u] + sz[u]; i++)
            add(rev[i], delta);
    }

    template <class Add, class Answer>
    void dfs(int u, bool keep, Add &add, Answer &answer) {
        for (int v : g[u])
            if (v != par[u] && v != heavy[u])
                dfs(v, false, add, answer);
        if (heavy[u])
            dfs(heavy[u], true, add, answer);
        for (int v : g[u])
            if (v != par[u] && v != heavy[u])
                add_subtree(v, +1, add);
        add(u, +1);
        answer(u);
        if (!keep)
            add_subtree(u, -1, add);
    }

    template <class Add, class Answer>
    void solve(int root, Add add, Answer answer) {
        dfs(root, true, add, answer);
    }
};
