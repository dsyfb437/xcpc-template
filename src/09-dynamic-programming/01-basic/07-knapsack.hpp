/**
 * @brief 0/1 背包与完全背包
 * @complexity O(nV)，空间 O(V)
 * @index 容量 0 到 V
 * @idea 一维 dp 记不超过当前容量的最大价值。0/1 背包倒序扫容量，完全背包正序扫。
 * @usage knapsack_01(items,V) 或 knapsack_unbounded(items,V)，物品为 {重量,价值}。
 * @pitfall 重量为正整数，允许空集与负价值。每个最优值及转移和需放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll knapsack_01(const vector<pair<int, ll>> &items, int V) {
    vector<ll> dp(V + 1);
    for (auto [weight, value] : items)
        for (int x = V; x >= weight; x--)
            dp[x] = max(dp[x], dp[x - weight] + value);
    return dp[V];
}

inline ll knapsack_unbounded(const vector<pair<int, ll>> &items, int V) {
    vector<ll> dp(V + 1);
    for (auto [weight, value] : items)
        for (int x = weight; x <= V; x++)
            dp[x] = max(dp[x], dp[x - weight] + value);
    return dp[V];
}
