/**
 * @brief 树上启发式合并（DSU on tree）
 * @complexity O(n log n)，单次加点、删点和记答案均为 O(1) 时
 * @index 点 1-based，DFS 序位置 0-based
 *
 * @idea 先算轻儿子，算完就清掉。再保留重儿子的数据，把轻子树和自己加回来。
 * @usage `DSUOnTree dsu(g);`
 *        `auto add = [&](int u, int delta) { cnt[color[u]] += delta; };`
 *        `auto answer = [&](int u) { ans[u] = cnt[color[u]]; };`
 *        `dsu.solve(root, add, answer);`
 * @pitfall g 是非空树，且在 dsu 使用期间有效。
 *          每次 solve 前清空统计量，结束后统计量里留着整棵树。
 *          树很深时留意递归栈。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DSUOnTree {
    int n, cur = 0;
    const vector<vector<int>> &g;
    vector<int> par, sz, heavy, pos, rev;

    explicit DSUOnTree(const vector<vector<int>> &g)
        : n((int)g.size() - 1), g(g), par(n + 1), sz(n + 1), heavy(n + 1, 0),
          pos(n + 1), rev(n) {}

    void dfs1(int u, int p) {
        par[u] = p;
        sz[u] = 1;
        heavy[u] = 0;
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
        cur = 0;
        dfs1(root, 0);
        dfs2(root);
        dfs(root, true, add, answer);
    }
};
