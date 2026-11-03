/**
 * @brief 构造（指定逆序对数的排列）
 * @complexity O(n)
 * @index 排列值 1 到 n
 * @idea 把最大数放在前面可贡献剩余长度减一对。最后一次把最大数插到恰好贡献 k 对的位置。
 * @usage `auto p = permutation_with_inversions(n, k);` 返回恰有 k 个逆序对的排列。
 * @pitfall n 至少为 1，k 不合法时返回空数组。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> permutation_with_inversions(int n, ll k) {
    if (n < 1 || k < 0 || k > 1LL * n * (n - 1) / 2)
        return {};

    vector<int> p;
    p.reserve(n);
    while (n > 1 && k >= n - 1) {
        p.push_back(n);
        k -= n - 1;
        n--;
    }
    int before = n - 1 - (int)k;
    for (int i = 1; i <= before; i++)
        p.push_back(i);
    p.push_back(n);
    for (int i = before + 1; i < n; i++)
        p.push_back(i);
    return p;
}
