/**
 * @brief Manacher
 * @complexity O(n)
 * @index 字符串下标 0-based
 *
 * d1[i]：以 i 为中心的最长奇回文半径（含中心），回文长度 2*d1[i]-1。
 * d2[i]：以 i-1、i 之间为中心的最长偶回文半径，回文长度 2*d2[i]。
 *
 * 例：
 *   auto [d1, d2] = manacher(s);
 *   // 以 i 为中心的最长回文长度：奇数 2*d1[i]-1，偶数 2*d2[i] */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

pair<vector<int>, vector<int>> manacher(const string &s) {
    int n = s.size();
    vector<int> d1(n), d2(n);
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k])
            k++;
        d1[i] = k--;
        if (i + k > r)
            l = i - k, r = i + k;
    }
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k])
            k++;
        d2[i] = k--;
        if (i + k > r)
            l = i - k - 1, r = i + k;
    }
    return {d1, d2};
}
