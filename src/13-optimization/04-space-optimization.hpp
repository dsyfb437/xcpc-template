/**
 * @brief 空间优化（0/1 背包只留一行）
 * @complexity O(nW) 时间，O(W) 空间
 * @index dp[w] 表示容量至多 w 时的最大价值
 * @idea 每件物品处理一轮，容量从大到小更新。这样读到的 dp[w-weight] 还是处理这件物品之前的值。
 * @usage auto dp = knapsack_one_row(items, capacity);
 *   long long best = dp[capacity];
 *   items 每项为 {重量, 价值}。
 * @pitfall 重量和容量非负，价值可以为负。需要恢复选了哪些物品时要另外记录。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> knapsack_one_row(const vector<pair<int, ll>> &items,
                                   int capacity) {
    vector<ll> dp(size_t(capacity) + 1);
    for (auto [weight, value] : items) {
        for (int w = capacity; w >= weight; w--)
            dp[w] = max(dp[w], dp[w - weight] + value);
    }
    return dp;
}
