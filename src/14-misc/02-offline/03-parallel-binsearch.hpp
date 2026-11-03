/**
 * @brief 整体二分（带单点修改的区间第 k 小）
 * @complexity 离散化 O((n+q) log(2+n+q))，二分 O((n+q) log(2+n) log(2+V))，空间 O(n+q)
 * @index 位置 0-based，区间 [l,r)，k 从 1 开始
 * @idea 按值域二分。按操作原顺序扫，用树状数组数当前区间里有多少值 ≤ mid。查询据此分到左右，去右边时扣掉这些数。
 * @usage vector<RangeKthOp> ops = {RangeKthQuery{0, 5, 3}, RangeKthChange{2, 9}};
 *   auto answer = offline_range_kth(a, ops);
 * @pitfall V 是不同值数。修改和查询一起传入，答案只按查询顺序返回。k 必须在 1 到区间长度之间。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct RangeKthChange {
    int pos;
    ll value;
};

struct RangeKthQuery {
    int l, r, k;
};

using RangeKthOp = variant<RangeKthChange, RangeKthQuery>;

inline vector<ll> offline_range_kth(const vector<ll> &a,
                                    const vector<RangeKthOp> &ops) {
    auto values = a;
    for (const auto &op : ops)
        if (const auto *change = get_if<RangeKthChange>(&op))
            values.push_back(change->value);
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    auto rank = [&](ll x) {
        return int(lower_bound(values.begin(), values.end(), x) - values.begin());
    };

    struct Event {
        int pos = 0, delta = 0, rank = 0;
        int l = 0, r = 0, k = 0, id = -1;
    };
    int n = (int)a.size(), queries = 0;
    vector<int> current(n);
    vector<Event> events;
    for (int i = 0; i < n; i++) {
        current[i] = rank(a[i]);
        events.push_back({.pos = i, .delta = 1, .rank = current[i]});
    }
    for (const auto &op : ops) {
        if (const auto *change = get_if<RangeKthChange>(&op)) {
            int i = change->pos;
            events.push_back({.pos = i, .delta = -1, .rank = current[i]});
            current[i] = rank(change->value);
            events.push_back({.pos = i, .delta = 1, .rank = current[i]});
        } else {
            auto [l, r, k] = get<RangeKthQuery>(op);
            events.push_back({.l = l, .r = r, .k = k, .id = queries++});
        }
    }
    if (queries == 0)
        return {};

    vector<int> bit(n + 1);
    vector<ll> answer(queries);
    vector<Event> scratch(events.size());
    vector<bool> lower(events.size());
    auto add = [&](int pos, int delta) {
        for (int i = pos + 1; i <= n; i += i & -i)
            bit[i] += delta;
    };
    auto prefix = [&](int end) {
        int total = 0;
        for (int i = end; i > 0; i -= i & -i)
            total += bit[i];
        return total;
    };
    auto solve = [&](auto &&self, int l, int r, int lo, int hi) -> void {
        if (l == r)
            return;
        if (lo == hi) {
            for (int i = l; i < r; i++)
                if (events[i].id != -1)
                    answer[events[i].id] = values[lo];
            return;
        }

        int mid = lo + (hi - lo) / 2, left_count = 0;
        for (int i = l; i < r; i++) {
            auto &e = events[i];
            if (e.id == -1) {
                lower[i] = e.rank <= mid;
                if (lower[i])
                    add(e.pos, e.delta);
            } else {
                int count = prefix(e.r) - prefix(e.l);
                lower[i] = e.k <= count;
                if (!lower[i])
                    e.k -= count;
            }
            left_count += lower[i];
        }

        int split = l + left_count, left = l, right = split;
        for (int i = l; i < r; i++) {
            const auto &e = events[i];
            if (lower[i] && e.id == -1)
                add(e.pos, -e.delta);
            scratch[lower[i] ? left++ : right++] = e;
        }
        copy(scratch.begin() + l, scratch.begin() + r, events.begin() + l);
        self(self, l, split, lo, mid);
        self(self, split, r, mid + 1, hi);
    };
    solve(solve, 0, (int)events.size(), 0, (int)values.size() - 1);
    return answer;
}
