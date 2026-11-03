/**
 * @brief 区间 DP（石子合并）
 * @complexity O(n^3)，空间 O(n^2)
 * @index 区间 [l,r)
 * @idea 枚举最后一次合并的分割点。两边先各自合并，再加整个区间的重量。
 * @usage stone_merge_cost(a) 返回线性石子合并的最小代价。
 * @pitfall 只能合并相邻两堆。重量可为负，区间和与总成本用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline __int128 stone_merge_cost(const vector<ll> &a) {
    int n = (int)a.size();
    vector<__int128> prefix(n + 1);
    for (int i = 0; i < n; i++)
        prefix[i + 1] = prefix[i] + a[i];

    vector dp(n + 1, vector<__int128>(n + 1));
    for (int len = 2; len <= n; len++)
        for (int l = 0; l + len <= n; l++) {
            int r = l + len;
            __int128 best = dp[l][l + 1] + dp[l + 1][r];
            for (int k = l + 2; k < r; k++)
                best = min(best, dp[l][k] + dp[k][r]);
            dp[l][r] = best + prefix[r] - prefix[l];
        }
    return dp[0][n];
}
