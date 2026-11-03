/**
 * @brief 回滚莫队（相同值最远距离）
 * @complexity 排序 O(n log n + q log q)，移动 O((n+q)√n)
 * @index 区间 [l,r)，0-based
 * @idea 左端点同块的查询按右端点递增。右边只加不删，左边临时加到位，答完恢复。
 * @usage auto ans = rollback_mo_max_equal_gap(a, queries);
 *   ans[i] 是第 i 个区间内相同值的最大下标差，找不到一对就返回 0。
 * @pitfall 完全落在一个块里的查询直接枚举。临时加入只改窗口的状态，别把最优值留给下一次。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int>
rollback_mo_max_equal_gap(const vector<ll> &a,
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
        if (l1 / block != l2 / block)
            return l1 / block < l2 / block;
        return r1 < r2;
    });

    vector<int> first(values.size(), -1), last = first, direct = first;
    vector<pair<int, int>> history;
    int current_block = -1, boundary = 0, right = 0, best = 0;
    for (int id : order) {
        auto [l, r] = queries[id];
        if (l / block != current_block) {
            current_block = l / block;
            right = boundary = min(n, l - l % block + block);
            fill(first.begin(), first.end(), -1);
            fill(last.begin(), last.end(), -1);
            best = 0;
        }
        if (r <= boundary) {
            for (int i = l; i < r; i++) {
                int x = rank[i];
                if (direct[x] == -1)
                    direct[x] = i;
                ans[id] = max(ans[id], i - direct[x]);
            }
            for (int i = l; i < r; i++)
                direct[rank[i]] = -1;
            continue;
        }

        while (right < r) {
            int x = rank[right];
            if (first[x] == -1)
                first[x] = right;
            last[x] = right++;
            best = max(best, last[x] - first[x]);
        }
        int temporary_best = best;
        for (int i = boundary - 1; i >= l; i--) {
            int x = rank[i];
            history.emplace_back(x, first[x]);
            if (first[x] == -1)
                last[x] = i;
            first[x] = i;
            temporary_best = max(temporary_best, last[x] - i);
        }
        ans[id] = temporary_best;
        while (!history.empty()) {
            auto [x, old_first] = history.back();
            history.pop_back();
            first[x] = old_first;
            if (old_first == -1)
                last[x] = -1;
        }
    }
    return ans;
}
