/**
 * @brief ST 表（区间最值）
 * @complexity 预处理 O(n log n)，查询 O(1)
 * @index 数组 0-based，区间 [l, r)
 *
 * 默认求最小值；要最大值就传 greater<ll>。区间是 [l, r)。
 * 只支持可重复贡献的运算（min/max/gcd），不支持下标的加法。
 *
 * 例：
 *   SparseTable<ll> st(a);   // a 是 0-indexed 数组
 *   ll mn = st.query(l, r);  // min(a[l..r-1]) */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T, class Cmp = less<T>> struct SparseTable {
    vector<vector<T>> st;
    vector<int> lg;

    explicit SparseTable(const vector<T> &a) {
        int n = a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;
        int k = n ? lg[n] : 0;
        st.assign(k + 1, vector<T>(n));
        if (n == 0)
            return;
        st[0] = a;
        for (int j = 1; j <= k; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                st[j][i] = better(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
    }

    T query(int l, int r) const { // [l, r)
        int j = lg[r - l];
        return better(st[j][l], st[j][r - (1 << j)]);
    }

    static T better(const T &a, const T &b) { return Cmp()(a, b) ? a : b; }
};
