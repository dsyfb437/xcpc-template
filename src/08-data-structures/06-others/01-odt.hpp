/**
 * @brief 珂朵莉树（区间赋值与加法）
 * @complexity 单次 O(log n + k)，k 为访问的段数
 * @index 数组 0-based，区间 [l,r)
 * @idea map 只存等值段的起点。先把左右端点切成段边界，再整段操作。
 * @usage OrderedIntervals t(a); t.assign(l,r,x); t.add(l,r,d); t.sum(l,r)。
 * @pitfall 最坏会退化到逐元素。相邻同值段会合并，值的加法需放进 long long，区间和用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct OrderedIntervals {
    int n;
    map<int, ll> runs;
    using Iterator = map<int, ll>::iterator;

    explicit OrderedIntervals(const vector<ll> &a) : n((int)a.size()) {
        for (int i = 0; i < n; i++)
            if (!i || a[i - 1] != a[i])
                runs.emplace_hint(runs.end(), i, a[i]);
        runs.emplace_hint(runs.end(), n, 0);
    }

    Iterator split(int position) {
        auto it = runs.lower_bound(position);
        if (it->first == position)
            return it;
        return runs.emplace_hint(it, position, prev(it)->second);
    }

    void compact(int l, int r) {
        auto right = runs.find(r);
        if (r != n && right != runs.begin() && prev(right)->second == right->second)
            runs.erase(right);
        auto left = runs.find(l);
        if (left != runs.begin() && prev(left)->second == left->second)
            runs.erase(left);
    }

    void assign(int l, int r, ll value) {
        if (l == r)
            return;
        auto last = split(r), first = split(l);
        first->second = value;
        runs.erase(next(first), last);
        compact(l, r);
    }

    void add(int l, int r, ll delta) {
        if (l == r || !delta)
            return;
        auto last = split(r);
        for (auto it = split(l); it != last; ++it)
            it->second += delta;
        compact(l, r);
    }

    __int128 sum(int l, int r) const {
        if (l == r)
            return 0;
        __int128 answer = 0;
        auto it = prev(runs.upper_bound(l));
        while (it->first < r) {
            auto after = next(it);
            int len = min(after->first, r) - max(it->first, l);
            answer += (__int128)len * it->second;
            it = after;
        }
        return answer;
    }
};
