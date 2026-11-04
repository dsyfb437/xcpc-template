/**
 * @brief Entringer 数
 * @complexity O(n^2)
 * @index 三角表下标 0 到 n
 * @idea 按首项的大小分类交错排列。每行累加上一行反向的项，第 i 行求和就是长度 i + 1 的交错排列总数。
 * @usage auto e = entringer_table(n, mod); e[i][j] 数长度 i + 1、首项为 j + 1 且先降的交错排列。
 * @pitfall mod 为正 int，可为合数。长度 1 的单元素排列算一种。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<vector<int>> entringer_table(int n, int mod) {
    vector<vector<int>> e(n + 1);
    e[0] = {1 % mod};
    for (int i = 1; i <= n; i++) {
        e[i].resize(i + 1);
        for (int j = 1; j <= i; j++)
            e[i][j] = (e[i][j - 1] + (ll)e[i - 1][i - j]) % mod;
    }
    return e;
}
