/**
 * @brief 贪心（区间调度）
 * @complexity O(n log n)
 * @index 区间 [l, r)
 * @idea 每次选结束最早的可选区间。换成它只会给后面的区间留下更多位置。
 * @usage `auto chosen = schedule_intervals(intervals);` 返回最多能选的一组区间。
 * @pitfall 区间满足 l < r，端点相接可以同时选。每个区间收益相同。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<pair<int, int>> schedule_intervals(vector<pair<int, int>> a) {
    sort(a.begin(), a.end(), [](auto x, auto y) { return x.second < y.second; });
    vector<pair<int, int>> chosen;
    for (auto [l, r] : a)
        if (chosen.empty() || chosen.back().second <= l)
            chosen.emplace_back(l, r);
    return chosen;
}
