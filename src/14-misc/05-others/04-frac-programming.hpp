/**
 * @brief 分数规划（恰选 k 项，最大化总价值 / 总重量）
 * @complexity 每轮平均 O(n)，默认二分 80 轮
 * @index value[i] 与 weight[i] 对应
 * @idea 二分答案 x。选 k 个最大的 value[i]-x·weight[i]，和非负就能达到 x。
 * @usage auto ratio = max_k_ratio(value, weight, k);
 * @pitfall 重量严格为正，1 ≤ k ≤ n。用 long double，结果为近似值。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

inline ld max_k_ratio(const vector<ld> &value, const vector<ld> &weight, int k) {
    int n = (int)value.size();
    assert(0 < k && k <= n && value.size() == weight.size());
    vector<ld> adjusted(n);
    for (int i = 0; i < n; i++) {
        assert(weight[i] > 0);
        adjusted[i] = value[i] / weight[i];
    }
    auto [low, high] = minmax_element(adjusted.begin(), adjusted.end());
    ld l = *low, r = *high;

    for (int step = 0; step < 80; step++) {
        ld x = midpoint(l, r);
        if (x == l || x == r)
            break;
        for (int i = 0; i < n; i++)
            adjusted[i] = value[i] - x * weight[i];
        if (k < n) {
            nth_element(adjusted.begin(), adjusted.begin() + k, adjusted.end(),
                        greater<ld>());
        }
        ld sum = accumulate(adjusted.begin(), adjusted.begin() + k, 0.L);
        if (sum >= 0)
            l = x;
        else
            r = x;
    }
    return midpoint(l, r);
}
