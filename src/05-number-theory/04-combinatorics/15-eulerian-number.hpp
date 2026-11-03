/**
 * @brief Eulerian 数（按下降数）
 * @complexity O(n^2)
 * @index 下降数从 0 起
 * @idea 插入最大元素，若放在已有下降位置或末尾，下降数不变。放到其他位置就多一个下降。
 * @usage auto a = eulerian_table(n, mod); a[i][j] 数 i 元排列中恰有 j 个下降的方案。
 * @pitfall mod 为正 int，可为合数。a[0][0] 取 1，非空行只存 0 到 i - 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<vector<int>> eulerian_table(int n, int mod) {
    vector<vector<int>> a(n + 1);
    a[0] = {1 % mod};
    for (int i = 1; i <= n; i++) {
        a[i].resize(i);
        for (int j = 0; j < i; j++) {
            ll value = 0;
            if (j < (int)a[i - 1].size())
                value = 1LL * (j + 1) * a[i - 1][j] % mod;
            if (j > 0)
                value += 1LL * (i - j) * a[i - 1][j - 1] % mod;
            a[i][j] = value % mod;
        }
    }
    return a;
}
