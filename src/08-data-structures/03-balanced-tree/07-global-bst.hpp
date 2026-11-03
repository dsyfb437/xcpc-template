/**
 * @brief 全局平衡二叉树（静态树链求和）
 * @complexity 预处理 O(n log n)，单次 O(log n)
 * @index 原树点 1-based
 * @idea 按子树大小做重链剖分。每条链用带权中位数建二叉树，点权重为 1 加所有轻子树大小，跨链查询的代价可望远镜相消。
 * @usage GlobalBalancedTree t(g,values,root); t.set(u,x); t.path_sum(u,v)。
 * @pitfall 树的拓扑固定，values[0] 留空。值用 long long，路径和用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct GlobalBalancedTree {
    vector<int> parent, depth, head, left, right, up;
    vector<ll> value;
    vector<__int128> total;

    GlobalBalancedTree(const vector<vector<int>> &g, const vector<ll> &values,
                       int root = 1)
        : parent(g.size()), depth(g.size()), head(g.size()), left(g.size()),
          right(g.size()), up(g.size()), value(values), total(g.size()) {
        vector<int> order{root}, size(g.size(), 1), heavy(g.size());
        size[0] = 0;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : g[u])
                if (v != parent[u]) {
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    order.push_back(v);
                }
        }
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int u = order[i], p = parent[u];
            size[p] += size[u];
            if (size[u] > size[heavy[p]])
                heavy[p] = u;
        }

        for (int start : order) {
            if (heavy[parent[start]] == start)
                continue;
            vector<int> chain, prefix{0};
            for (int u = start; u; u = heavy[u]) {
                head[u] = start;
                chain.push_back(u);
                prefix.push_back(prefix.back() + size[u] - size[heavy[u]]);
            }
            auto build = [&](auto &&self, int l, int r, int p) -> int {
                if (l == r)
                    return 0;
                int half = prefix[l] + (prefix[r] - prefix[l] - 1) / 2;
                int mid = upper_bound(prefix.begin() + l + 1,
                                      prefix.begin() + r + 1, half) -
                          prefix.begin() - 1;
                int u = chain[mid];
                up[u] = p;
                left[u] = self(self, l, mid, u);
                right[u] = self(self, mid + 1, r, u);
                pull(u);
                return u;
            };
            build(build, 0, (int)chain.size(), 0);
        }
    }

    void pull(int u) { total[u] = total[left[u]] + value[u] + total[right[u]]; }

    void set(int u, ll x) {
        value[u] = x;
        for (; u; u = up[u])
            pull(u);
    }

    __int128 root_sum(int u) const {
        __int128 result = 0;
        while (u) {
            int start = head[u];
            result += total[left[u]] + value[u];
            for (int p = up[u]; p; u = p, p = up[u])
                if (right[p] == u)
                    result += total[left[p]] + value[p];
            u = parent[start];
        }
        return result;
    }

    int lca(int u, int v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]])
                swap(u, v);
            u = parent[head[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }

    __int128 path_sum(int u, int v) const {
        int w = lca(u, v);
        return root_sum(u) + root_sum(v) - 2 * root_sum(w) + value[w];
    }
};
