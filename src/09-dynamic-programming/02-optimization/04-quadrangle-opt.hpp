/**
 * @brief 四边形不等式优化（Knuth 石子合并）
 * @complexity O(n^2) 时间和空间
 * @index 区间 [l,r)
 * @idea 区间转移满足四边形不等式和区间包含单调性时，最优分点被两侧短一区间的分点夹住。
 * @usage knuth_stone_merge(a) 求相邻石子合并的最小成本。
 * @pitfall 本模板要求石堆重量非负。区间和与总成本用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline __int128 knuth_stone_merge(const vector<ll> &a) {
    int n = (int)a.size();
    vector<__int128> prefix(n + 1);
    vector dp(n + 1, vector<__int128>(n + 1));
    vector opt(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + a[i];
        opt[i][i + 1] = i + 1;
    }
    for (int len = 2; len <= n; len++)
        for (int l = 0; l + len <= n; l++) {
            int r = l + len;
            int low = max(l + 1, opt[l][r - 1]);
            int high = min(r - 1, opt[l + 1][r]);
            int at = low;
            auto best = dp[l][low] + dp[low][r];
            for (int k = low + 1; k <= high; k++) {
                auto cost = dp[l][k] + dp[k][r];
                if (cost < best) {
                    best = cost;
                    at = k;
                }
            }
            opt[l][r] = at;
            dp[l][r] = best + prefix[r] - prefix[l];
        }
    return dp[0][n];
}
