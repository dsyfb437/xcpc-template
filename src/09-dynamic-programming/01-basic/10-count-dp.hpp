/**
 * @brief 计数 DP（硬币凑和）
 * @complexity O(nV)，空间 O(V)
 * @index 目标金额 0 到 V
 * @idea 先枚举面值，再正序枚举金额。这样每组硬币多重集只按一种顺序被计入。
 * @usage coin_change_counts(coins,V,mod) 返回凑出每个金额的方案数。
 * @pitfall 面值为正且互异，每种不限数量。mod 为正 int，金额 0 的空选法算一种。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> coin_change_counts(const vector<int> &coins, int V, int mod) {
    vector<int> ways(V + 1);
    ways[0] = 1 % mod;
    for (int coin : coins)
        for (int x = coin; x <= V; x++)
            ways[x] = (ways[x] + (ll)ways[x - coin]) % mod;
    return ways;
}
