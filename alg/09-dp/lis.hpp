/**
 * @brief 最长上升子序列（LIS）
 * @complexity O(n log n)
 * @index 数组 0-based
 *
 * 返回 LIS 长度。默认严格上升；传 false 求非降。
 * 例：
 *   int len = lis(a);          // 严格上升
 *   int len2 = lis(a, false);  // 非降
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int lis(const vector<ll> &a, bool strict = true) {
    vector<ll> d; // d[i]：长度为 i+1 的上升子序列的最小结尾
    for (ll x : a) {
        auto it = strict ? lower_bound(d.begin(), d.end(), x)
                         : upper_bound(d.begin(), d.end(), x);
        if (it == d.end())
            d.push_back(x);
        else
            *it = x;
    }
    return (int)d.size();
}
