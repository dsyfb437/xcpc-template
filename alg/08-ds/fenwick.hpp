/**
 * @brief 树状数组
 * @complexity O(log n)
 * @index 下标 1-based
 *
 * 单点加、前缀和、区间和，以及树上二分第 k 小。
 * kth(k) 要求所有元素非负且总和 >= k。
 *
 * 例：
 *   Fenwick bit(n);
 *   bit.add(i, x);           // a[i] += x
 *   ll s = bit.range(l, r);  // a[l] + ... + a[r] */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Fenwick {
    int n;
    vector<ll> t;

    explicit Fenwick(int n) : n(n), t(n + 1) {}

    void add(int i, ll v) {
        for (; i <= n; i += i & -i)
            t[i] += v;
    }

    ll sum(int i) const {
        ll s = 0;
        for (; i > 0; i -= i & -i)
            s += t[i];
        return s;
    }

    ll range(int l, int r) const { return l > r ? 0 : sum(r) - sum(l - 1); }

    // 最小的 i 使前缀和 >= k。
    int kth(ll k) const {
        int pos = 0;
        ll cur = 0;
        int step = 1;
        while (step * 2 <= n)
            step *= 2;
        for (; step; step >>= 1) {
            if (pos + step <= n && cur + t[pos + step] < k) {
                pos += step;
                cur += t[pos];
            }
        }
        return pos + 1;
    }
};
