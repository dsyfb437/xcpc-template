/**
 * @brief 单调队列优化（多重背包）
 * @complexity O(nV)，空间 O(V)
 * @index 容量 0 到 V
 * @idea 按容量对重量取模分组。沿同余组转移时，候选值减去已走步数的价值后，变成滑动窗口最大值。
 * @usage bounded_knapsack(items,V) 返回总重不超过 V 的最大价值，物品为 {重量,价值,最多件数}。
 * @pitfall 重量为正，件数非负，允许空集和负价值。用 __int128 防止转移乘法溢出。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline __int128 bounded_knapsack(const vector<tuple<int, ll, int>> &items, int V) {
    vector<__int128> dp(V + 1);
    for (auto [weight, value, limit] : items) {
        vector<__int128> next(V + 1);
        for (int r = 0; r < weight && r <= V; r++) {
            auto score = [&](int j) {
                return dp[r + j * weight] - (__int128)j * value;
            };
            deque<int> candidates;
            for (int j = 0; j <= (V - r) / weight; j++) {
                while (!candidates.empty() && candidates.front() < j - limit)
                    candidates.pop_front();
                auto current = score(j);
                while (!candidates.empty() && score(candidates.back()) <= current)
                    candidates.pop_back();
                candidates.push_back(j);
                next[r + j * weight] =
                    score(candidates.front()) + (__int128)j * value;
            }
        }
        dp = move(next);
    }
    return dp[V];
}
