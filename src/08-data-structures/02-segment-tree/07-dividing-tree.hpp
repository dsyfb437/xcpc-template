/**
 * @brief 划分树（静态区间第 k 小）
 * @complexity 预处理 O(n log n)，查询 O(log n)
 * @index 区间 [l,r)，k 从 1 起
 * @idea 按中位数稳定分到两边，相等项按配额分。每层用前缀计数把查询区间映射到下一层。
 * @usage DividingTree t(a); t.kth(l,r,k) 返回区间第 k 小。
 * @pitfall a 非空，允许重复值，k 必须在 1 到 r-l。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DividingTree {
    int n;
    vector<vector<ll>> values;
    vector<vector<int>> prefix;
    vector<ll> sorted;

    explicit DividingTree(vector<ll> a)
        : n((int)a.size()), values(bit_width((unsigned)n) + 1, vector<ll>(n)),
          prefix(values.size(), vector<int>(n)), sorted(a) {
        assert(n > 0);
        sort(sorted.begin(), sorted.end());
        values[0] = move(a);
        auto build = [&](auto &&self, int depth, int l, int r) -> void {
            if (r - l == 1)
                return;
            int m = (l + r) / 2, quota = m - l;
            ll pivot = sorted[m - 1];
            for (int i = l; i < r; i++)
                quota -= values[depth][i] < pivot;
            int left = l, right = m;
            for (int i = l; i < r; i++) {
                ll x = values[depth][i];
                if (x < pivot || (x == pivot && quota > 0)) {
                    values[depth + 1][left++] = x;
                    quota -= x == pivot;
                } else {
                    values[depth + 1][right++] = x;
                }
                prefix[depth][i] = left - l;
            }
            self(self, depth + 1, l, m);
            self(self, depth + 1, m, r);
        };
        build(build, 0, 0, n);
    }

    ll kth(int l, int r, int k) const {
        assert(0 <= l && l < r && r <= n && 1 <= k && k <= r - l);
        int a = 0, b = n;
        for (int d = 0; b - a > 1; d++) {
            int m = (a + b) / 2;
            int before = l == a ? 0 : prefix[d][l - 1];
            int through = prefix[d][r - 1];
            int count = through - before;
            if (k <= count) {
                l = a + before;
                r = a + through;
                b = m;
            } else {
                l = m + l - a - before;
                r = m + r - a - through;
                a = m;
                k -= count;
            }
        }
        return sorted[a];
    }
};
