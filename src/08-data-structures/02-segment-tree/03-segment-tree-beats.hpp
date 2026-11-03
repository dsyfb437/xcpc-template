/**
 * @brief Segment Tree Beats（区间取 min）
 * @complexity n 项上 m 次操作 O((n + m) log n)
 * @index 数组 0-based，区间 [l,r)
 * @idea 存最大值、严格次大值和最大值次数。上限严格大于次大值时，只改最大值那一组。
 * @usage SegmentTreeBeats t(a); t.chmin(l,r,x); t.sum(l,r); t.maximum(l,r)。
 * @pitfall a 非空，最大值查询要求非空区间。区间和用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegmentTreeBeats {
    struct Node {
        __int128 sum;
        ll max1, max2;
        int count;
    };
    int n;
    vector<Node> tree;

    explicit SegmentTreeBeats(const vector<ll> &a) : n((int)a.size()), tree(4 * n) {
        build(1, 0, n, a);
    }

    void pull(int u) {
        auto a = tree[2 * u], b = tree[2 * u + 1];
        if (a.max1 < b.max1)
            swap(a, b);
        if (a.max1 == b.max1)
            tree[u] = {a.sum + b.sum, a.max1, max(a.max2, b.max2),
                       a.count + b.count};
        else
            tree[u] = {a.sum + b.sum, a.max1, max(a.max2, b.max1), a.count};
    }

    void build(int u, int l, int r, const vector<ll> &a) {
        if (r - l == 1) {
            tree[u] = {a[l], a[l], LLONG_MIN, 1};
            return;
        }
        int m = (l + r) / 2;
        build(2 * u, l, m, a);
        build(2 * u + 1, m, r, a);
        pull(u);
    }

    void limit(int u, ll value) {
        auto &a = tree[u];
        if (a.max1 > value) {
            a.sum -= ((__int128)a.max1 - value) * a.count;
            a.max1 = value;
        }
    }

    void push(int u) {
        limit(2 * u, tree[u].max1);
        limit(2 * u + 1, tree[u].max1);
    }

    void chmin(int first, int last, ll value) {
        auto dfs = [&](auto &&self, int u, int l, int r) -> void {
            if (r <= first || last <= l || tree[u].max1 <= value)
                return;
            if (first <= l && r <= last && (r - l == 1 || tree[u].max2 < value)) {
                limit(u, value);
                return;
            }
            push(u);
            int m = (l + r) / 2;
            self(self, 2 * u, l, m);
            self(self, 2 * u + 1, m, r);
            pull(u);
        };
        dfs(dfs, 1, 0, n);
    }

    pair<__int128, ll> query(int first, int last) {
        auto dfs = [&](auto &&self, int u, int l, int r) -> pair<__int128, ll> {
            if (r <= first || last <= l)
                return {0, LLONG_MIN};
            if (first <= l && r <= last)
                return {tree[u].sum, tree[u].max1};
            push(u);
            int m = (l + r) / 2;
            auto [a, x] = self(self, 2 * u, l, m);
            auto [b, y] = self(self, 2 * u + 1, m, r);
            return {a + b, max(x, y)};
        };
        return dfs(dfs, 1, 0, n);
    }

    __int128 sum(int l, int r) { return query(l, r).first; }
    ll maximum(int l, int r) { return query(l, r).second; }
};
