/**
 * @brief 动态点分治（最近标记点）
 * @complexity 建树 O(n log n)，修改 O(log^2 n)，查询 O(log n)
 * @index 点 1-based
 *
 * @idea 给每个点记下它到各级分治重心的距离。每个重心维护标记点的距离集合，
 *       查询时枚举这些重心，取两段距离和的最小值。
 * @usage NearestMarked marked(g); marked.set_marked(u, true);
 *        marked.set_marked(u, false) 取消标记。marked.nearest(v) 查距离。
 * @pitfall g 是非空无权无向树，初始全未标记。没有标记点时返回 -1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct NearestMarked {
    vector<vector<pair<int, int>>> ancestors;
    vector<multiset<int>> distances;
    vector<bool> marked;

    explicit NearestMarked(const vector<vector<int>> &g)
        : ancestors(g.size()), distances(g.size()), marked(g.size()) {
        vector<bool> removed(g.size());
        vector<int> parent(g.size()), size(g.size()), distance(g.size());
        auto build = [&](auto &&self, int entry) -> void {
            vector<int> order{entry};
            parent[entry] = 0;
            for (int i = 0; i < (int)order.size(); i++) {
                int u = order[i];
                size[u] = 1;
                for (int v : g[u])
                    if (!removed[v] && v != parent[u]) {
                        parent[v] = u;
                        order.push_back(v);
                    }
            }

            int total = (int)order.size(), center = entry;
            for (int i = total - 1; i > 0; i--) {
                int u = order[i];
                size[parent[u]] += size[u];
                if (size[u] > total / 2 && center == entry)
                    center = u;
            }

            order = {center};
            parent[center] = 0;
            distance[center] = 0;
            for (int i = 0; i < total; i++) {
                int u = order[i];
                ancestors[u].emplace_back(center, distance[u]);
                for (int v : g[u])
                    if (!removed[v] && v != parent[u]) {
                        parent[v] = u;
                        distance[v] = distance[u] + 1;
                        order.push_back(v);
                    }
            }

            removed[center] = true;
            for (int v : g[center])
                if (!removed[v])
                    self(self, v);
        };
        build(build, 1);
    }

    void set_marked(int u, bool value) {
        if (marked[u] == value)
            return;
        marked[u] = value;
        for (auto [center, d] : ancestors[u]) {
            auto &s = distances[center];
            if (value)
                s.insert(d);
            else
                s.erase(s.find(d));
        }
    }

    int nearest(int u) const {
        ll best = numeric_limits<ll>::max();
        for (auto [center, d] : ancestors[u])
            if (!distances[center].empty())
                best = min(best, 0LL + d + *distances[center].begin());
        return best == numeric_limits<ll>::max() ? -1 : (int)best;
    }
};
