/**
 * @brief 任务排序（最小化加权完成时间）
 * @complexity O(n log n) 时间，O(n) 空间
 * @index 返回任务的 0-based 编号
 * @idea 只有一台机器，所有任务一开始就能做。交换相邻任务可知，weight / time 大的先做。比较时交叉相乘。
 * @usage auto order = smith_job_order(jobs);
 *   jobs 每项是 {time, weight}，目标是最小化 Σ weight × 完成时刻。
 * @pitfall 耗时严格为正、权重非负。任务不可中断。比值相同的任务按输入顺序排。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> smith_job_order(const vector<pair<ll, ll>> &jobs) {
    vector<int> order(jobs.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        auto [ti, wi] = jobs[i];
        auto [tj, wj] = jobs[j];
        __int128 left = (__int128)wi * tj, right = (__int128)wj * ti;
        if (left != right)
            return left > right;
        return i < j;
    });
    return order;
}
