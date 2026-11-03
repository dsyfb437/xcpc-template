/**
 * @brief 树套树（离线二维树状数组）
 * @complexity 单次 O(log^2 n)
 * @index x 为 0-based，矩形半开
 * @idea 外层按 x 更新树状数组，每个外层节点把可能出现的 y 离散化，维护一棵内层树状数组。
 * @usage Fenwick2D t(nx,points); t.add(x,y,delta); t.sum(x1,y1,x2,y2) 查矩形和。
 * @pitfall 建树时列出所有会修改的坐标，初值为零。x 端点在 [0,nx]，y 可为任意 int。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Fenwick2D {
    int n;
    vector<vector<int>> ys;
    vector<vector<ll>> bit;

    Fenwick2D(int n, const vector<pair<int, int>> &points)
        : n(n), ys(n + 1), bit(n + 1) {
        for (auto [x, y] : points)
            for (int i = x + 1; i <= n; i += i & -i)
                ys[i].push_back(y);
        for (int i = 1; i <= n; i++) {
            auto &a = ys[i];
            sort(a.begin(), a.end());
            a.erase(unique(a.begin(), a.end()), a.end());
            bit[i].resize(a.size() + 1);
        }
    }

    void add(int x, int y, ll delta) {
        for (int i = x + 1; i <= n; i += i & -i) {
            const auto &a = ys[i];
            auto it = lower_bound(a.begin(), a.end(), y);
            assert(it != a.end() && *it == y);
            for (int j = (int)(it - a.begin()) + 1; j < (int)bit[i].size();
                 j += j & -j)
                bit[i][j] += delta;
        }
    }

    ll prefix(int x, int y) const {
        ll value = 0;
        for (int i = x; i > 0; i -= i & -i) {
            const auto &a = ys[i];
            int j = lower_bound(a.begin(), a.end(), y) - a.begin();
            for (; j > 0; j -= j & -j)
                value += bit[i][j];
        }
        return value;
    }

    ll sum(int x1, int y1, int x2, int y2) const {
        return prefix(x2, y2) - prefix(x1, y2) - prefix(x2, y1) + prefix(x1, y1);
    }
};
