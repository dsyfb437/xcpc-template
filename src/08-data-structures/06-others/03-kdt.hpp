/**
 * @brief 二维 K-D Tree（静态最近点）
 * @complexity 建树 O(n log n)，查询最坏 O(n)
 * @index 点编号按输入从 0 起
 * @idea 交替按 x、y 切中位数，每个子树存包围盒。先搜离查询点更近的盒，按盒到点的距离下界剪枝。
 * @usage KDTree t(points); t.nearest(x,y,exclude) 返回 {距离平方,原编号}。
 * @pitfall 整数坐标绝对值不超过 10^12。exclude 默认 -1，无候选返回 nullopt。相同距离可返回任一点。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct KDTree {
    struct Node {
        ll x, y;
        int id;
        int child[2]{-1, -1};
        ll min_x, max_x, min_y, max_y;
    };
    vector<Node> nodes;
    int root;

    explicit KDTree(const vector<pair<ll, ll>> &points) {
        for (int i = 0; i < (int)points.size(); i++) {
            auto [x, y] = points[i];
            nodes.push_back({x, y, i, {-1, -1}, x, x, y, y});
        }
        root = build(0, (int)nodes.size(), 0);
    }

    int build(int l, int r, int axis) {
        if (l == r)
            return -1;
        int m = (l + r) / 2;
        nth_element(nodes.begin() + l, nodes.begin() + m, nodes.begin() + r,
                    [axis](const Node &a, const Node &b) {
                        return axis ? a.y < b.y : a.x < b.x;
                    });
        auto &u = nodes[m];
        u.child[0] = build(l, m, axis ^ 1);
        u.child[1] = build(m + 1, r, axis ^ 1);
        for (int v : u.child)
            if (v != -1) {
                u.min_x = min(u.min_x, nodes[v].min_x);
                u.max_x = max(u.max_x, nodes[v].max_x);
                u.min_y = min(u.min_y, nodes[v].min_y);
                u.max_y = max(u.max_y, nodes[v].max_y);
            }
        return m;
    }

    optional<pair<__int128, int>> nearest(ll x, ll y, int exclude = -1) const {
        constexpr __int128 inf = (__int128)1 << 120;
        auto square = [](ll value) { return (__int128)value * value; };
        auto lower_bound = [&](int v) -> __int128 {
            if (v == -1)
                return inf;
            auto &a = nodes[v];
            return square(x - clamp(x, a.min_x, a.max_x)) +
                   square(y - clamp(y, a.min_y, a.max_y));
        };
        __int128 best = inf;
        int id = -1;
        auto dfs = [&](auto &&self, int v) -> void {
            const auto &u = nodes[v];
            __int128 distance = square(x - u.x) + square(y - u.y);
            if (u.id != exclude && distance < best) {
                best = distance;
                id = u.id;
            }
            int a = u.child[0], b = u.child[1];
            auto da = lower_bound(a), db = lower_bound(b);
            if (da > db) {
                swap(a, b);
                swap(da, db);
            }
            if (da < best)
                self(self, a);
            if (db < best)
                self(self, b);
        };
        if (root != -1)
            dfs(dfs, root);
        if (id == -1)
            return nullopt;
        return pair{best, id};
    }
};
