/**
 * @brief 莫队二次离线（区间逆序对）
 * @complexity 值域分块预处理 O(n√V)，端点移动 O((n+q)√n)，另加排序
 * @index 区间 [l,r)，0-based
 * @idea 令 F(l,r) 为 i<l、j<r 且 a[i]>a[j] 的对数。端点每动一段，就记下一笔前缀排名查询，等扫到该前缀时一起算。
 *   左端点移动时，数右前缀里比它小的数。右端点移动时，数左前缀里比它大的数。
 * @usage auto ans = mo_inversion_counts(a, queries);
 *   queries 每项是 {l,r}，返回 i<j 且 a[i]>a[j] 的个数。
 * @pitfall V 是离散后不同值数。区间逆序数 = 前缀逆序数之差 - F(l,r) + F(l,l)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> mo_inversion_counts(const vector<ll> &a,
                                      const vector<pair<int, int>> &queries) {
    int n = (int)a.size();
    auto values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    vector<int> rank(n);
    for (int i = 0; i < n; i++)
        rank[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();

    int block = max(1, (int)sqrt(n));
    vector<int> order(queries.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        auto [l1, r1] = queries[i];
        auto [l2, r2] = queries[j];
        int b1 = l1 / block, b2 = l2 / block;
        if (b1 != b2)
            return b1 < b2;
        return b1 % 2 == 0 ? r1 < r2 : r1 > r2;
    });

    struct Move {
        int l, r, sign, id;
        bool count_smaller;
    };
    vector<vector<Move>> at_prefix(n + 1);
    auto record = [&](int old, int next, int prefix, int id, bool smaller) {
        if (old < next)
            at_prefix[prefix].push_back({old, next, 1, id, smaller});
        if (old > next)
            at_prefix[prefix].push_back({next, old, -1, id, smaller});
    };
    int l = 0, r = 0;
    for (int id : order) {
        auto [next_l, next_r] = queries[id];
        record(l, next_l, r, id, true);
        record(r, next_r, next_l, id, false);
        l = next_l;
        r = next_r;
    }

    // 插入改后续块和本块后续值，查询时只做一次加法。
    int kinds = (int)values.size(), width = max(1, (int)sqrt(kinds));
    vector<int> before_block(kinds / width + 1), before_value(kinds + 1);
    auto less_than = [&](int x) {
        return before_block[x / width] + before_value[x];
    };
    auto insert = [&](int x) {
        int b = x / width;
        for (int k = b + 1; k < (int)before_block.size(); k++)
            before_block[k]++;
        for (int y = x + 1; y <= kinds && y / width == b; y++)
            before_value[y]++;
    };

    vector<ll> delta(queries.size()), ans(queries.size());
    vector<ll> inversions(n + 1), unequal(n + 1);
    for (int p = 0; p <= n; p++) {
        for (const auto &step : at_prefix[p]) {
            ll total = 0;
            for (int i = step.l; i < step.r; i++) {
                int x = rank[i];
                total += step.count_smaller ? less_than(x) : p - less_than(x + 1);
            }
            delta[step.id] += step.sign * total;
        }
        if (p == n)
            break;
        int x = rank[p];
        inversions[p + 1] = inversions[p] + p - less_than(x + 1);
        unequal[p + 1] = unequal[p] + p - (less_than(x + 1) - less_than(x));
        insert(x);
    }

    ll f = 0;
    for (int id : order) {
        auto [left, right] = queries[id];
        f += delta[id];
        ans[id] = inversions[right] - inversions[left] - f + unequal[left];
    }
    return ans;
}
