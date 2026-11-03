/**
 * @brief Boyer–Moore 字符串匹配
 * @complexity 预处理 O(m + 256)，匹配最坏 O(nm)
 * @index 返回起点 0-based
 * @idea 从模式末尾向前比。失配后结合坏字符和好后缀规则，尽量向右跳过不可能匹配的位置。
 * @usage boyer_moore(text,pattern) 返回所有匹配位置，允许重叠。
 * @pitfall 按字节比较。空模式匹配 0 到 text.size()。此版未加 Galil 规则。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> boyer_moore(string_view text, string_view pattern) {
    int n = (int)text.size(), m = (int)pattern.size();
    if (!m) {
        vector<int> result(n + 1);
        iota(result.begin(), result.end(), 0);
        return result;
    }

    array<int, 256> last;
    last.fill(-1);
    for (int i = 0; i < m; i++)
        last[(unsigned char)pattern[i]] = i;

    vector<int> border(m + 1), shift(m + 1);
    int i = m, j = m + 1;
    border[i] = j;
    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (!shift[j])
                shift[j] = j - i;
            j = border[j];
        }
        border[--i] = --j;
    }
    j = border[0];
    for (i = 0; i <= m; i++) {
        if (!shift[i])
            shift[i] = j;
        if (i == j)
            j = border[j];
    }

    vector<int> result;
    for (int start = 0; start <= n - m;) {
        int k = m - 1;
        while (k >= 0 && pattern[k] == text[start + k])
            k--;
        if (k < 0) {
            result.push_back(start);
            start += shift[0];
        } else {
            int bad = k - last[(unsigned char)text[start + k]];
            start += max(shift[k + 1], bad);
        }
    }
    return result;
}
