/**
 * @brief 递归与分治（最大子段和）
 * @complexity O(n log n)
 * @index 区间 [l, r)，0-based
 * @idea 答案要么全在一侧，要么由左半的后缀和右半的前缀拼成。
 * @usage `auto ans = max_subarray_dc(a, 0, a.size());` 返回非空子段的最大和。
 * @pitfall 要求 l < r。全是负数时也要选一个数，所有子段和需放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll max_subarray_dc(const vector<int> &a, int l, int r) {
    if (r - l == 1)
        return a[l];

    int mid = l + (r - l) / 2;
    ll left = numeric_limits<ll>::lowest(), right = left;
    ll sum = 0;
    for (int i = mid - 1; i >= l; i--) {
        sum += a[i];
        left = max(left, sum);
    }
    sum = 0;
    for (int i = mid; i < r; i++) {
        sum += a[i];
        right = max(right, sum);
    }
    return max(
        {max_subarray_dc(a, l, mid), max_subarray_dc(a, mid, r), left + right});
}
