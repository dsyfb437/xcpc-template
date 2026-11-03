/**
 * @brief SG 函数
 * @complexity O(n+m)，m 是合法操作总数
 * @index 状态从 0 编号，后继编号必须小于当前状态
 * @idea 一个状态的 SG 值是所有后继 SG 值的 mex。几个独立子游戏每回合只能动其中一个时，把它们的 SG 值异或。
 * @usage auto sg = grundy_numbers(moves);
 *   只有一个游戏时，sg[start] != 0 就是先手必胜。
 * @pitfall 适用于双方走法相同、无环、走不了就输的游戏。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> grundy_numbers(const vector<vector<int>> &moves) {
    int n = (int)moves.size();
    vector<int> sg(n), last_seen(n + 1, -1);
    for (int u = 0; u < n; u++) {
        for (int v : moves[u])
            last_seen[sg[v]] = u;
        while (last_seen[sg[u]] == u)
            sg[u]++;
    }
    return sg;
}
