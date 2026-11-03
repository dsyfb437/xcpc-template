/**
 * @brief 最小表示法（最小循环移位）
 * @complexity O(n)，额外空间 O(1)
 * @index 返回原串起点 0-based
 * @idea 比较两条循环移位，第一次失配后，较大的一侧能跳过整段已经相同的前缀。
 * @usage minimum_rotation(s) 返回字典序最小的循环移位起点。
 * @pitfall 按无符号字节比较，多个最优起点取最小，空串返回 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int minimum_rotation(string_view s) {
    int n = (int)s.size();
    if (n == 0)
        return 0;
    int i = 0, j = 1, same = 0;
    while (i < n && j < n && same < n) {
        unsigned char a = s[(i + same) % n], b = s[(j + same) % n];
        if (a == b) {
            same++;
            continue;
        }
        if (a > b) {
            i += same + 1;
            if (i == j)
                i++;
        } else {
            j += same + 1;
            if (j == i)
                j++;
        }
        same = 0;
    }
    return min(i, j);
}
