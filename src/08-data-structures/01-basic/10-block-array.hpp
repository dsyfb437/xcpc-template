/**
 * @brief 块状数组（区间加、计数）
 * @complexity 修改 O(sqrt(n) log n)，查询 O(sqrt(n) log n)
 * @index 数组 0-based，区间 [l,r)
 * @idea 每块留一份排序后的副本。整块加用标记，边角改完重新排序。查整块时二分。
 * @usage SortedBlocks a(values); a.add(l,r,d); a.count_le(l,r,x) 统计不大于 x 的个数。
 * @pitfall 区间端点合法，允许空区间。元素、标记和真实值须放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SortedBlocks {
    int n, width;
    vector<ll> values, lazy;
    vector<vector<ll>> sorted;

    explicit SortedBlocks(const vector<ll> &a)
        : n((int)a.size()), width(max(1, (int)sqrt(n))), values(a),
          lazy((n + width - 1) / width), sorted(lazy.size()) {
        for (int i = 0; i < (int)sorted.size(); i++)
            rebuild(i);
    }

    void rebuild(int b) {
        int l = b * width, r = min(n, l + width);
        sorted[b].assign(values.begin() + l, values.begin() + r);
        sort(sorted[b].begin(), sorted[b].end());
    }

    void add(int l, int r, ll delta) {
        while (l < r) {
            int b = l / width, end = min(n, (b + 1) * width);
            if (l % width == 0 && end <= r) {
                lazy[b] += delta;
            } else {
                for (int i = l; i < min(end, r); i++)
                    values[i] += delta;
                rebuild(b);
            }
            l = end;
        }
    }

    int count_le(int l, int r, ll x) const {
        int result = 0;
        while (l < r) {
            int b = l / width, end = min(n, (b + 1) * width);
            if (l % width == 0 && end <= r) {
                const auto &a = sorted[b];
                __int128 target = (__int128)x - lazy[b];
                result += upper_bound(a.begin(), a.end(), target) - a.begin();
            } else {
                for (int i = l; i < min(end, r); i++)
                    result += (__int128)values[i] + lazy[b] <= x;
            }
            l = end;
        }
        return result;
    }
};
