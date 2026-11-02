/**
 * @brief KMP
 * @complexity O(n + m)
 * @index 字符串下标 0-based
 *
 * pi[i] 是 s[0..i] 最长的相等真前后缀长度，pi[0] = 0。
 * match 返回 pattern 在 text 中所有出现的起始下标。
 *
 * 例：
 *   vector<int> pi = prefix_function(p);
 *   vector<int> pos = kmp_match(text, p);  // p 出现的所有起点 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> prefix_function(const string &s) {
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            j++;
        pi[i] = j;
    }
    return pi;
}

vector<int> kmp_match(const string &text, const string &pattern) {
    int n = text.size(), m = pattern.size();
    vector<int> res;
    if (m == 0)
        return res;
    vector<int> pi = prefix_function(pattern);
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j])
            j = pi[j - 1];
        if (text[i] == pattern[j])
            j++;
        if (j == m) {
            res.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return res;
}
