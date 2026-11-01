/**
 * @brief 线段树（区间加、区间和）
 * @complexity build O(n)，单次 O(log n)
 * @index 下标 1-based
 *
 * 下标从 1 开始。n 是区间右端点，建树时传 a[1..n]。
 * 需要区间乘再加、区间最值的时候照这个结构改 apply 和 push 即可。
 *
 * 例：
 *   SegTree seg(n);
 *   seg.build(a);            // a[1..n]
 *   seg.add_range(l, r, v);
 *   ll s = seg.query(l, r); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree {
    int n;
    vector<ll> sum, add;

    explicit SegTree(int n) : n(n), sum(4 * n + 4), add(4 * n + 4) {}

    void build(int p, int l, int r, const vector<ll> &a) {
        add[p] = 0;
        if (l == r) {
            sum[p] = a[l];
            return;
        }
        int m = (l + r) >> 1;
        build(p * 2, l, m, a);
        build(p * 2 + 1, m + 1, r, a);
        sum[p] = sum[p * 2] + sum[p * 2 + 1];
    }

    void build(const vector<ll> &a) { build(1, 1, n, a); }

    void apply(int p, int l, int r, ll v) {
        sum[p] += v * (r - l + 1);
        add[p] += v;
    }

    void push(int p, int l, int r) {
        if (!add[p] || l == r)
            return;
        int m = (l + r) >> 1;
        apply(p * 2, l, m, add[p]);
        apply(p * 2 + 1, m + 1, r, add[p]);
        add[p] = 0;
    }

    void add_range(int p, int l, int r, int ql, int qr, ll v) {
        if (ql <= l && r <= qr) {
            apply(p, l, r, v);
            return;
        }
        push(p, l, r);
        int m = (l + r) >> 1;
        if (ql <= m)
            add_range(p * 2, l, m, ql, qr, v);
        if (qr > m)
            add_range(p * 2 + 1, m + 1, r, ql, qr, v);
        sum[p] = sum[p * 2] + sum[p * 2 + 1];
    }

    void add_range(int l, int r, ll v) {
        if (l <= r)
            add_range(1, 1, n, l, r, v);
    }

    ll query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr)
            return sum[p];
        push(p, l, r);
        int m = (l + r) >> 1;
        ll res = 0;
        if (ql <= m)
            res += query(p * 2, l, m, ql, qr);
        if (qr > m)
            res += query(p * 2 + 1, m + 1, r, ql, qr);
        return res;
    }

    ll query(int l, int r) { return l > r ? 0 : query(1, 1, n, l, r); }
};
