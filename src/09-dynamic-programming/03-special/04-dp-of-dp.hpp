/**
 * @brief DP 套 DP（按 LCS 长度计数）
 * @complexity O((m + 1) × (sigma + 1) × 2^n)，空间 O(2^n)
 * @index 返回数组按 LCS 长度 0 到 n
 * @idea LCS 一行相邻项只差 0 或 1，用位集记差分。外层按生成字符串长度做 DP，追加字符后用位运算更新整行。
 * @usage lcs_length_counts(pattern,alphabet,m,mod) 数每种 LCS 长度下的长度 m 字符串。
 * @pitfall pattern 长度 n 至多 20。alphabet 的字符互异，sigma 是字符数，mod 为正 int。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> lcs_length_counts(string_view pattern, string_view alphabet,
                                     int m, int mod) {
    int n = (int)pattern.size();
    unsigned states = 1U << n;
    array<unsigned, 256> match{};
    for (int i = 0; i < n; i++)
        match[(unsigned char)pattern[i]] |= 1U << i;

    vector<int> dp(states), next(states);
    dp[0] = 1 % mod;
    for (int len = 0; len < m; len++) {
        for (int &x : next)
            x = 0;
        for (unsigned mask = 0; mask < states; mask++) {
            if (!dp[mask])
                continue;
            unsigned shifted = (mask << 1) | 1;
            for (unsigned char c : alphabet) {
                unsigned x = match[c] | mask;
                unsigned row = x & ~(x - shifted);
                next[row] = (next[row] + (ll)dp[mask]) % mod;
            }
        }
        dp.swap(next);
    }

    vector<int> answer(n + 1);
    for (unsigned mask = 0; mask < states; mask++) {
        int count = popcount(mask);
        answer[count] = (answer[count] + (ll)dp[mask]) % mod;
    }
    return answer;
}
