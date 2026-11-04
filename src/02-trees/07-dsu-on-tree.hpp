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

    void dfs1(int root, int parent) {
        vector<int> order{root};
        par[root] = parent;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            sz[u] = 1;
            heavy[u] = 0;
            for (int v : g[u]) {
                if (v == par[u]) continue;
                par[v] = u;
                order.push_back(v);
            }
        }
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int u = order[i], p = par[u];
            sz[p] += sz[u];
            if (sz[u] >= sz[heavy[p]]) heavy[p] = u;
        }
    }

    void dfs2(int root) {
        vector<int> stack{root};
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            pos[u] = cur;
            rev[cur++] = u;
            for (auto it = g[u].rbegin(); it != g[u].rend(); ++it) {
                int v = *it;
                if (v != par[u] && v != heavy[u]) stack.push_back(v);
            }
            if (heavy[u]) stack.push_back(heavy[u]);
        }
    }

    template <class Add> void add_subtree(int u, int delta, Add &add) const {
        for (int i = pos[u]; i < pos[u] + sz[u]; i++)
            add(rev[i], delta);
    }

    template <class Add, class Answer>
    void dfs(int u, bool keep, Add &add, Answer &answer) {
        struct Visit { int u; bool keep, finish; };
        vector<Visit> stack{{u, keep, false}};
        while (!stack.empty()) {
            auto [x, retain, finish] = stack.back();
            stack.pop_back();
            if (!finish) {
                // 先算轻儿子，再算重儿子，最后回来合并。
                stack.push_back({x, retain, true});
                if (heavy[x]) stack.push_back({heavy[x], true, false});
                for (auto it = g[x].rbegin(); it != g[x].rend(); ++it) {
                    int v = *it;
                    if (v != par[x] && v != heavy[x])
                        stack.push_back({v, false, false});
                }
                continue;
            }
            for (int v : g[x])
                if (v != par[x] && v != heavy[x]) add_subtree(v, +1, add);
            add(x, +1);
            answer(x);
            if (!retain) add_subtree(x, -1, add);
        }
    }

    template <class Add, class Answer>
    void solve(int root, Add add, Answer answer) {
        cur = 0;
        dfs1(root, 0);
        dfs2(root);
        dfs(root, true, add, answer);
    }
};
