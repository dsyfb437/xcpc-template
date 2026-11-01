/**
 * @brief 单调栈 / 单调队列
 * @complexity O(n)
 * @index 数组下标 0-based
 *
 * 单调栈：求每个位置左边（或右边）第一个更小/更大的位置。
 * 单调队列：定长滑动窗口的最值。
 * 例：
 *   vector<int> L = previous_less(a);   // 左边第一个 < a[i] 的下标，没有为 -1
 *   vector<int> R = next_greater(a);    // 右边第一个 > a[i] 的下标
 *   vector<ll> mn = window_min(a, k);   // 每个长度 k 窗口的最小值
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 左边第一个严格小于 a[i] 的位置
inline vector<int> previous_less(const vector<ll> &a) {
    int n = a.size();
    vector<int> res(n, -1), st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && a[st.back()] >= a[i])
            st.pop_back();
        if (!st.empty())
            res[i] = st.back();
        st.push_back(i);
    }
    return res;
}

// 右边第一个严格大于 a[i] 的位置
inline vector<int> next_greater(const vector<ll> &a) {
    int n = a.size();
    vector<int> res(n, -1), st;
    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && a[st.back()] <= a[i])
            st.pop_back();
        if (!st.empty())
            res[i] = st.back();
        st.push_back(i);
    }
    return res;
}

// 每个长度 k 的窗口的最小值，返回 n-k+1 个
inline vector<ll> window_min(const vector<ll> &a, int k) {
    deque<int> q;
    vector<ll> res;
    for (int i = 0; i < (int)a.size(); i++) {
        while (!q.empty() && a[q.back()] >= a[i])
            q.pop_back();
        q.push_back(i);
        if (q.front() <= i - k)
            q.pop_front();
        if (i >= k - 1)
            res.push_back(a[q.front()]);
    }
    return res;
}
