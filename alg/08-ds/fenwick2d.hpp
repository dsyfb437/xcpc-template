/**
 * @brief 二维树状数组
 * @complexity O(log^2 n) 单点加 / 前缀和
 * @index 下标 1-based
 *
 * 单点加、子矩阵和（用前缀和容斥）。下标从 1 开始。
 * 例：
 *   Fenwick2D bit(n, m);
 *   bit.add(x, y, v);
 *   ll s = bit.sum(x2, y2) - bit.sum(x1 - 1, y2) - bit.sum(x2, y1 - 1) + bit.sum(x1 - 1, y1 - 1);
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Fenwick2D {
    int n, m;
    vector<vector<ll>> t;

    Fenwick2D(int n, int m) : n(n), m(m), t(n + 1, vector<ll>(m + 1, 0)) {}

    void add(int x, int y, ll v) {
        for (int i = x; i <= n; i += i & -i)
            for (int j = y; j <= m; j += j & -j)
                t[i][j] += v;
    }

    // (1,1) 到 (x,y) 的和
    ll sum(int x, int y) const {
        ll s = 0;
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j)
                s += t[i][j];
        return s;
    }
};
