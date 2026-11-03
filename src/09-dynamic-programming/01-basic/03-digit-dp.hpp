/**
 * @brief 数位 DP（无重复数字计数）
 * @complexity O(D × 2^10 × 10)
 * @index 统计闭区间 [0,n]
 * @idea 从最高位往下填，记录已用数字和是否贴着上界。前导零不占用数字 0，只记忆化非贴边状态。
 * @usage count_distinct_digits(n) 返回十进制各位互异的整数个数，包含 0。
 * @pitfall n 为无符号 64 位整数。改成别的题时替换 mask 和转移条件。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline ull count_distinct_digits(ull n) {
    string digits = to_string(n);
    int length = (int)digits.size();
    vector<array<ull, 1 << 10>> memo(length);
    for (auto &row : memo)
        row.fill(UINT64_MAX);

    auto dfs = [&](auto &&self, int pos, int mask, bool tight) -> ull {
        if (pos == length)
            return 1;
        if (!tight && memo[pos][mask] != UINT64_MAX)
            return memo[pos][mask];
        int limit = tight ? digits[pos] - '0' : 9;
        ull answer = 0;
        for (int d = 0; d <= limit; d++) {
            if (mask >> d & 1)
                continue;
            int next = !mask && !d ? 0 : mask | (1 << d);
            answer += self(self, pos + 1, next, tight && d == limit);
        }
        if (!tight)
            memo[pos][mask] = answer;
        return answer;
    };
    return dfs(dfs, 0, 0, true);
}
