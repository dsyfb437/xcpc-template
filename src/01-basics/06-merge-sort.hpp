/**
 * @brief 归并排序 / 逆序对
 * @complexity O(n log n)
 * @index 数组 0-based
 *
 * @idea 返回逆序对数（i < j 且 a[i] > a[j] 的对数）。
 * @usage 例：`ll inv = count_inversions(a);`
 * 
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll merge_count(vector<ll> &a, vector<ll> &tmp, int l, int r) {
    if (r - l <= 1)
        return 0;
    int m = (l + r) >> 1;
    ll res = merge_count(a, tmp, l, m) + merge_count(a, tmp, m, r);
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
            res += m - i; // a[i..m-1] 都比 a[j] 大
        }
    }
    while (i < m)
        tmp[k++] = a[i++];
    while (j < r)
        tmp[k++] = a[j++];
    for (int t = l; t < r; t++)
        a[t] = tmp[t];
    return res;
}

inline ll count_inversions(vector<ll> a) {
    vector<ll> tmp(a.size());
    return merge_count(a, tmp, 0, (int)a.size());
}
