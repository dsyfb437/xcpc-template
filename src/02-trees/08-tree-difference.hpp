/**
 * @brief 树上差分
 * @complexity 建树 O(n)，已知 LCA 后单次修改 O(1)，汇总 O(n)
 * @index 点 1-based，根的父亲为 0
 *
 * @idea 在路径两端各加一次，再在 LCA 附近扣掉多算的部分。
 *       所有修改做完后，从儿子往父亲累加。
 * @usage `TreeDifference diff(g, root);`
 *        `int w = lca.lca(u, v);`
 *        `diff.add_vertices(u, v, w, delta);` 给路径上的点加 delta。
 *        `diff.add_edges(u, v, w, delta);` 给路径上的边加 delta。
 *        `auto ans = diff.collect();` 边权模式下 ans[u] 对应 u 到父亲的边。
 * @pitfall w 是同一个 root 下 u、v 的 LCA，由调用方传入。
 *          g 是非空树。点权、边权分别用两个实例，累加值需放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct TreeDifference {
    vector<int> parent, order;
    vector<ll> diff;

    explicit TreeDifference(const vector<vector<int>> &g, int root = 1)
        : parent(g.size()), order{root}, diff(g.size()) {
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : g[u]) {
                if (v == parent[u])
                    continue;
                parent[v] = u;
                order.push_back(v);
            }
        }
    }

    void add_vertices(int u, int v, int w, ll delta) {
        diff[u] += delta;
        diff[v] += delta;
        diff[w] -= delta;
        diff[parent[w]] -= delta;
    }

    void add_edges(int u, int v, int w, ll delta) {
        diff[u] += delta;
        diff[v] += delta;
        diff[w] -= delta;
        diff[w] -= delta;
    }

    vector<ll> collect() const {
        auto ans = diff;
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int u = order[i];
            ans[parent[u]] += ans[u];
        }
        ans[0] = 0;
        return ans;
    }
};
