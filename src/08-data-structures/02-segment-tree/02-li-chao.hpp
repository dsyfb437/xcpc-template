/**
 * @brief 离散李超树（最小值）
 * @complexity 整条直线 O(log n)，线段 O(log^2 n)，查询 O(log n)
 * @index 横坐标按升序离散化
 * @idea 节点保留在中点更优的直线，另一条只可能在一侧更优，递归放进那一侧。
 * @usage LiChao tree(xs); tree.add({k,b}); tree.add_segment({k,b},l,r); tree.query(i)。
 * @pitfall xs 非空、升序且无重复，线段用下标区间 [l,r)。没有覆盖返回 nullopt，直线值用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LiChao {
    struct Line {
        ll slope, intercept;
        __int128 at(ll x) const { return (__int128)slope * x + intercept; }
    };
    vector<ll> xs;
    vector<optional<Line>> best;

    explicit LiChao(vector<ll> coordinates)
        : xs(move(coordinates)), best(4 * xs.size()) {
        assert(!xs.empty());
    }

    void insert(Line line, int node, int l, int r) {
        auto &current = best[node];
        if (!current) {
            current = line;
            return;
        }
        int mid = (l + r) / 2;
        bool left = line.at(xs[l]) < current->at(xs[l]);
        bool middle = line.at(xs[mid]) < current->at(xs[mid]);
        if (middle)
            swap(line, *current);
        if (r - l == 1)
            return;
        if (left != middle)
            insert(line, 2 * node, l, mid);
        else
            insert(line, 2 * node + 1, mid, r);
    }

    void add(Line line) { insert(line, 1, 0, (int)xs.size()); }

    void add_segment(Line line, int first, int last) {
        if (first == last)
            return;
        auto dfs = [&](auto &&self, int node, int l, int r) -> void {
            if (first <= l && r <= last) {
                insert(line, node, l, r);
                return;
            }
            int mid = (l + r) / 2;
            if (first < mid)
                self(self, node * 2, l, mid);
            if (mid < last)
                self(self, node * 2 + 1, mid, r);
        };
        dfs(dfs, 1, 0, (int)xs.size());
    }

    optional<__int128> query(int i) const {
        optional<__int128> answer;
        for (int node = 1, l = 0, r = (int)xs.size();;) {
            if (best[node]) {
                auto value = best[node]->at(xs[i]);
                if (!answer || value < *answer)
                    answer = value;
            }
            if (r - l == 1)
                return answer;
            int mid = (l + r) / 2;
            if (i < mid) {
                node *= 2;
                r = mid;
            } else {
                node = 2 * node + 1;
                l = mid;
            }
        }
    }
};
