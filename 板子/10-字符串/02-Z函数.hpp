/**
 * @brief Z 函数（扩展 KMP）
 * @complexity O(n)
 * @index 字符串下标 0-based
 *
 * z[i] = lcp(s, s[i..])，这里约定 z[0] = n。
 * 找 pattern 在 text 中的出现：拼 pattern + '#' + text 跑一遍即可。
 *
 * 例：
 *   vector<int> z = z_function(s); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> z_function(const string &s) {
    int n = s.size();
    vector<int> z(n, 0);
    if (n == 0)
        return z;
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r)
            z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}
