/**
 * @brief 倍增
 * @complexity 建表 O(n log K)，查询 O(log K)
 * @index 状态 0-based
 * @idea up[j][u] 表示从 u 走 2^j 步，下一层把同一层的跳转做两遍。
 * @usage `JumpTable jump(next, K);` 用 `jump.go(u, k)` 求 k 步后的状态。
 * @pitfall next[u] 必须是有效状态。查询步数 k 不超过建表时的 K。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct JumpTable {
    vector<vector<int>> up;

    JumpTable(const vector<int> &next, ull max_steps)
        : up(bit_width(max_steps), next) {
        for (int j = 1; j < (int)up.size(); j++)
            for (int u = 0; u < (int)next.size(); u++)
                up[j][u] = up[j - 1][up[j - 1][u]];
    }

    int go(int u, ull k) const {
        for (int j = 0; k; j++, k >>= 1)
            if (k & 1)
                u = up[j][u];
        return u;
    }
};
