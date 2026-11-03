/**
 * @brief 普通莫队（区间不同数个数）
 * @complexity 排序 O(n log n + q log q)，移动 O((n+q)√n)
 * @index 区间 [l,r)，0-based
 * @idea 左端点分块。同一块里右端点蛇形排序，移动端点时只更新进出区间的数。
 * @usage auto ans = mo_distinct(a, queries);
 *   queries 每项是 {l,r}，答案按输入顺序返回。
 * @pitfall 窗口初始为空。先扩张再收缩，避免删掉窗口之外的数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> mo_distinct(const vector<ll> &a,
                               const vector<pair<int, int>> &queries) {
    int n = (int)a.size();
    auto values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    vector<int> rank(n);
    for (int i = 0; i < n; i++)
        rank[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();

    int block = max(1, (int)sqrt(n));
    vector<int> order(queries.size()), ans(queries.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        auto [l1, r1] = queries[i];
        auto [l2, r2] = queries[j];
        int b1 = l1 / block, b2 = l2 / block;
        if (b1 != b2)
            return b1 < b2;
        return b1 % 2 == 0 ? r1 < r2 : r1 > r2;
    });

    vector<int> frequency(values.size());
    int distinct = 0, l = 0, r = 0;
    auto add = [&](int i) { distinct += frequency[rank[i]]++ == 0; };
    auto del = [&](int i) { distinct -= --frequency[rank[i]] == 0; };
    for (int id : order) {
        auto [left, right] = queries[id];
        while (l > left)
            add(--l);
        while (r < right)
            add(r++);
        while (l < left)
            del(l++);
        while (r > right)
            del(--r);
        ans[id] = distinct;
    }
    return ans;
}
