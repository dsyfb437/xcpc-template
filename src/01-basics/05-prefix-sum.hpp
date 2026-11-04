/**
 * @brief 前缀和 / 差分
 * @complexity O(n)
 * @index 数组 0-based
 *
 * 前缀和：pre[i] = a[0] + ... + a[i-1]，区间 [l, r] 的和是 pre[r+1] - pre[l]。
 * 差分：diff[0] = a[0]，diff[i] = a[i] - a[i-1]；区间加 v 只要 diff[l] += v、
 * 当 r+1 < n 时再做 diff[r+1] -= v，然后前缀和还原。
 * @pitfall difference 返回长度为 n 的数组。前缀和、相邻差和修改后的中间结果都要能放进 long long。
 * 例：
 *   `auto pre = prefix_sum(a);`
 *   `ll s = pre[r + 1] - pre[l];`
 * 
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> prefix_sum(const vector<ll> &a) {
    vector<ll> pre(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); i++)
        pre[i + 1] = pre[i] + a[i];
    return pre;
}

inline vector<ll> difference(const vector<ll> &a) {
    vector<ll> d(a.size());
    for (size_t i = 0; i < a.size(); i++)
        d[i] = a[i] - (i ? a[i - 1] : 0);
    return d;
}

inline vector<ll> restore(const vector<ll> &d) {
    vector<ll> a(d.size());
    ll s = 0;
    for (size_t i = 0; i < d.size(); i++)
        a[i] = s += d[i];
    return a;
}
