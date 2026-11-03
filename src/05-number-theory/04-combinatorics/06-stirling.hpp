/**
 * @brief 斯特林数
 * @complexity O(n^2)
 * @index 下标 0 到 n
 * @idea 第一类把新元素单独成环或插进已有环。第二类把新元素单独成组或放进某个已有组。
 * @usage stirling_table(n, mod) 给出第二类。第三个参数传 true 得到无符号第一类。
 * @pitfall mod 为正 int，可以是合数。两类都取 S[0][0] = 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<vector<int>> stirling_table(int n, int mod, bool first = false) {
    vector<vector<int>> s(n + 1);
    s[0] = {1 % mod};
    for (int i = 1; i <= n; i++) {
        s[i].resize(i + 1);
        for (int j = 1; j <= i; j++) {
            ll ways = s[i - 1][j - 1];
            if (j < i)
                ways += 1LL * (first ? i - 1 : j) * s[i - 1][j];
            s[i][j] = ways % mod;
        }
    }
    return s;
}
