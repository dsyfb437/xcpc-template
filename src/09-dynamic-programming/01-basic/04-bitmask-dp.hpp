/**
 * @brief 状压 DP（最小费用指派）
 * @complexity O(n 2^n)，空间 O(2^n)
 * @index 人和任务 0-based
 * @idea mask 记已选任务。已选数就是下一位人的编号，枚举没选过的任务转移。
 * @usage assignment_dp(cost) 返回每人分配一个不同任务的最小总费用。
 * @pitfall cost 是 int 方阵，允许负费用。建议 n <= 22，大图用匈牙利算法。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll assignment_dp(const vector<vector<int>> &cost) {
    int n = (int)cost.size(), states = 1 << n;
    vector<ll> dp(states, LLONG_MAX);
    dp[0] = 0;
    for (int mask = 0; mask + 1 < states; mask++) {
        int u = popcount((unsigned)mask);
        unsigned remaining = (states - 1) ^ mask;
        for (; remaining; remaining &= remaining - 1) {
            int v = countr_zero(remaining);
            int next = mask | (1 << v);
            dp[next] = min(dp[next], dp[mask] + cost[u][v]);
        }
    }
    return dp.back();
}
