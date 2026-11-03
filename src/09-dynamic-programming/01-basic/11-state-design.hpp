/**
 * @brief 状态压缩（编辑距离滚动数组）
 * @complexity O(nm) 时间，O(min(n,m)) 空间
 * @index dp 按较短字符串的前缀编号
 * @idea 编辑距离只依赖上一行、当前行左边和左上角。用一行和一个旧对角值就能原地转移。
 * @usage edit_distance(a,b) 返回最少插入、删除和替换次数。
 * @pitfall 三种操作代价均为 1。字符按字节比较，允许空串。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int edit_distance(string_view a, string_view b) {
    if (a.size() < b.size())
        swap(a, b);
    vector<int> dp(b.size() + 1);
    for (int j = 0; j < (int)dp.size(); j++)
        dp[j] = j;
    for (char ch : a) {
        int diagonal = dp[0]++;
        for (int j = 1; j < (int)dp.size(); j++) {
            int old = dp[j];
            dp[j] = min({old + 1, dp[j - 1] + 1, diagonal + (ch != b[j - 1])});
            diagonal = old;
        }
    }
    return dp.back();
}
