/**
 * @brief 一般图最大权匹配（状压 DP）
 * @complexity O(n 2^n)
 * @index 点 0-based
 * @idea 固定剩余点中编号最小的 u，枚举跳过 u 或让 u 与一个剩余邻点配对。
 * @usage auto [weight, mate] = weighted_matching(w); mate[u] 为配对点，-1 表示未匹配。
 * @pitfall 适合 n <= 22。w 为对称矩阵，缺边填 0，非正权边可跳过，匹配可以为空。最大总权值须放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<ll, vector<int>> weighted_matching(const vector<vector<ll>> &w) {
    int n = (int)w.size();
    unsigned states = 1U << n;
    vector<ll> dp(states);
    vector<int> choice(states, -1);
    for (unsigned mask = 1; mask < states; mask++) {
        int u = countr_zero(mask);
        unsigned rest = mask & (mask - 1);
        dp[mask] = dp[rest];
        for (unsigned bits = rest; bits; bits &= bits - 1) {
            int v = countr_zero(bits);
            ll candidate = dp[rest ^ (1U << v)] + w[u][v];
            if (w[u][v] > 0 && candidate > dp[mask]) {
                dp[mask] = candidate;
                choice[mask] = v;
            }
        }
    }

    vector<int> mate(n, -1);
    for (unsigned mask = states - 1; mask;) {
        int u = countr_zero(mask), v = choice[mask];
        mask &= mask - 1;
        if (v != -1) {
            mate[u] = v;
            mate[v] = u;
            mask ^= 1U << v;
        }
    }
    return {dp.back(), move(mate)};
}
