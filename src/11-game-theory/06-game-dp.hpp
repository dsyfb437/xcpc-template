/**
 * @brief 博弈 DP（先手是否必胜）
 * @complexity O(n+m)
 * @index 后继编号必须小于当前状态
 * @idea 只要能一步走到对手必败的状态，当前就必胜。没有合法走法时当前必败。
 * @usage auto win = game_win_states(moves);
 *   bool first_wins = win[start];
 * @pitfall moves[u] 写从 u 出发的一步走法。双方合法走法相同，平局和有环的图要另处理。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<bool> game_win_states(const vector<vector<int>> &moves) {
    vector<bool> win(moves.size());
    for (int u = 0; u < (int)moves.size(); u++) {
        for (int v : moves[u]) {
            if (!win[v]) {
                win[u] = true;
                break;
            }
        }
    }
    return win;
}
