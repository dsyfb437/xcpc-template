/**
 * @brief 序列自动机（位置表）
 * @complexity 建表 O(n)，匹配 O(m log n)
 * @index 位置 0-based
 * @idea 把每个字节的出现位置存成升序表。匹配时二分找不早于当前位置的最早出现处。
 * @usage SubsequenceAutomaton seq(text);
 *   int pos = seq.next(start, ch);  // 从 start 起找 ch 的位置
 *   int end = seq.match(pattern, start);  // 返回最后一个位置加一
 * @pitfall 按字节处理，找不到返回 -1。start 为含端点起点，空模式直接返回 start。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SubsequenceAutomaton {
    array<vector<int>, 256> position;

    explicit SubsequenceAutomaton(string_view text) {
        for (int i = 0; i < (int)text.size(); i++)
            position[(unsigned char)text[i]].push_back(i);
    }

    int next(int start, unsigned char ch) const {
        const auto &a = position[ch];
        auto it = lower_bound(a.begin(), a.end(), start);
        return it == a.end() ? -1 : *it;
    }

    int match(string_view pattern, int start = 0) const {
        for (unsigned char c : pattern) {
            int at = next(start, c);
            if (at == -1)
                return -1;
            start = at + 1;
        }
        return start;
    }
};
