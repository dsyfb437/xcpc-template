/**
 * @brief 长链剖分
 * @complexity O(n)
 * @index 点 1-based，序号 0-based
 * @idea 每个点选高度最大的儿子接成长链。先访问这个儿子，同一条链在序里连续。
 * @usage LongChains chains(g, root)，读 heavy、head、pos、order、height。
 * @pitfall height 按点数计，叶子为 1。g 是非空树。重儿子的深度 DP 可复用父亲后一格。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LongChains {
    vector<int> heavy, head, pos, order, height;

    LongChains(const vector<vector<int>> &g, int root = 1)
        : heavy(g.size()), head(g.size()), pos(g.size()), height(g.size(), 1) {
        vector<int> parent(g.size()), bfs{root};
        for (int i = 0; i < (int)bfs.size(); i++) {
            int u = bfs[i];
            for (int v : g[u])
                if (v != parent[u]) {
                    parent[v] = u;
                    bfs.push_back(v);
                }
        }
        for (int i = (int)bfs.size() - 1; i > 0; i--) {
            int u = bfs[i], p = parent[u];
            if (height[u] + 1 > height[p]) {
                height[p] = height[u] + 1;
                heavy[p] = u;
            }
        }
        vector<int> starts{root};
        while (!starts.empty()) {
            int top = starts.back();
            starts.pop_back();
            for (int u = top; u; u = heavy[u]) {
                head[u] = top;
                pos[u] = (int)order.size();
                order.push_back(u);
                for (int v : g[u])
                    if (v != parent[u] && v != heavy[u])
                        starts.push_back(v);
            }
        }
    }
};
