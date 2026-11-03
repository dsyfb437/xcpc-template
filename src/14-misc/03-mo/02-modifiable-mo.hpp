/**
 * @brief 带修改莫队（区间不同数个数）
 * @complexity n、修改数和查询数同阶时 O(n^(5/3)) 时间
 * @index 位置 0-based，查询 [l,r)
 * @idea 左端点块、右端点块、修改时间依次排序。时间往前就应用修改，往回就撤销。
 * @usage vector<MoOperation> ops = {MoChange{2, 9}, MoQuery{0, 5}};
 *   auto ans = mo_distinct_with_updates(a, ops);
 * @pitfall 改到窗口内的元素时，先移出旧值，再加进新值。答案只按查询顺序返回。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MoChange {
    int pos;
    ll value;
};
struct MoQuery {
    int l, r;
};
using MoOperation = variant<MoChange, MoQuery>;

inline vector<int> mo_distinct_with_updates(const vector<ll> &a,
                                            const vector<MoOperation> &ops) {
    auto values = a;
    for (const auto &op : ops)
        if (const auto *c = get_if<MoChange>(&op))
            values.push_back(c->value);
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    auto rank = [&](ll x) {
        return int(lower_bound(values.begin(), values.end(), x) - values.begin());
    };
    vector<int> initial;
    for (ll x : a)
        initial.push_back(rank(x));

    struct Change {
        int pos, before, after;
    };
    struct Query {
        int l, r, time, id;
    };
    vector<Change> changes;
    vector<Query> queries;
    auto current = initial;
    for (const auto &op : ops) {
        if (const auto *c = get_if<MoChange>(&op)) {
            int pos = c->pos, value = rank(c->value);
            changes.push_back({pos, current[pos], value});
            current[pos] = value;
        } else {
            auto [l, r] = get<MoQuery>(op);
            queries.push_back({l, r, (int)changes.size(), (int)queries.size()});
        }
    }

    int block = max(1, (int)pow(a.size(), 2.0 / 3));
    sort(queries.begin(), queries.end(), [&](const Query &x, const Query &y) {
        int xl = x.l / block, yl = y.l / block;
        if (xl != yl)
            return xl < yl;
        int xr = x.r / block, yr = y.r / block;
        if (xr != yr)
            return xr < yr;
        return x.time < y.time;
    });

    current = initial;
    vector<int> frequency(values.size()), ans(queries.size());
    int l = 0, r = 0, time = 0, distinct = 0;
    auto add = [&](int i) { distinct += frequency[current[i]]++ == 0; };
    auto del = [&](int i) { distinct -= --frequency[current[i]] == 0; };
    auto apply = [&](const Change &c, int value) {
        bool inside = l <= c.pos && c.pos < r;
        if (inside)
            del(c.pos);
        current[c.pos] = value;
        if (inside)
            add(c.pos);
    };
    for (const auto &q : queries) {
        while (time < q.time) {
            apply(changes[time], changes[time].after);
            time++;
        }
        while (time > q.time) {
            time--;
            apply(changes[time], changes[time].before);
        }
        while (l > q.l)
            add(--l);
        while (r < q.r)
            add(r++);
        while (l < q.l)
            del(l++);
        while (r > q.r)
            del(--r);
        ans[q.id] = distinct;
    }
    return ans;
}
