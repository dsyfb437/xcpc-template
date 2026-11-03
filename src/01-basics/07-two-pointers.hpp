/**
 * @brief 双指针 / 尺取
 * @complexity O(n)
 * @index 数组 0-based
 * @idea 右端每次加一项，和超限就从左边删，留下最长的合法后缀。
 * @usage `int len = longest_window(a, limit);` 返回和不超过 limit 的最长长度。
 * @pitfall 元素非负，窗口和用 long long 保存。没有合法非空窗口时返回 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int longest_window(const vector<int> &a, ll limit) {
    int left = 0, best = 0;
    ll sum = 0;
    for (int right = 0; right < (int)a.size(); right++) {
        sum += a[right];
        while (left <= right && sum > limit)
            sum -= a[left++];
        best = max(best, right - left + 1);
    }
    return best;
}
