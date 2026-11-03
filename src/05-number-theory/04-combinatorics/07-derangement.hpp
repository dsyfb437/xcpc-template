/**
 * @brief 错排数
 * @complexity O(n)
 * @index 下标 0 到 n
 * @idea 第一个元素有 i - 1 个去处。和它形成二元环，或把它接入已有的环，得到两项递推。
 * @usage derangement_table(n, mod) 返回 D[0] 到 D[n]。
 * @pitfall mod 为正 int，可为合数。空排列算一种。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> derangement_table(int n, int mod) {
    vector<int> d(n + 1);
    d[0] = 1 % mod;
    for (int i = 2; i <= n; i++)
        d[i] = (i - 1LL) * (d[i - 1] + (ll)d[i - 2]) % mod;
    return d;
}
