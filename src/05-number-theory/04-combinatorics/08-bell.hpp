/**
 * @brief 贝尔数
 * @complexity O(n^2)，空间 O(n)
 * @index 下标 0 到 n
 * @idea 用 Bell 三角形。每行首项接上一行末项，后面每格等于左边加上一行左上格。
 * @usage bell_table(n, mod) 返回 0 到 n 个带编号元素的集合分拆数。
 * @pitfall mod 为正 int，可为合数。B[0] = 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> bell_table(int n, int mod) {
    vector<int> answer(n + 1), row{1 % mod};
    answer[0] = row[0];
    for (int i = 1; i <= n; i++) {
        vector<int> next(i + 1);
        next[0] = row.back();
        for (int j = 1; j <= i; j++)
            next[j] = (next[j - 1] + (ll)row[j - 1]) % mod;
        answer[i] = next[0];
        row = move(next);
    }
    return answer;
}
