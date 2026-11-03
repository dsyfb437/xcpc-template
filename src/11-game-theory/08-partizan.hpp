/**
 * @brief 非公平组合游戏（双方走法不同）
 * @complexity O(n+m)，m 是双方走法总数
 * @index 后继编号必须小于当前状态
 * @idea 分别算左方先走、右方先走时左方能不能赢。左方找能赢的一步，右方找能让左方输的一步。
 * @usage auto result = partizan_outcomes(left_moves, right_moves);
 *   result[u].left_starts 和 result[u].right_starts 都是问左方能不能赢。
 * @pitfall 双方轮流走，走不了就输。几个子游戏一起玩时，状态要包含所有子局面。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PartizanOutcome {
    bool left_starts = false;
    bool right_starts = true;
};

inline vector<PartizanOutcome>
partizan_outcomes(const vector<vector<int>> &left_moves,
                  const vector<vector<int>> &right_moves) {
    vector<PartizanOutcome> result(left_moves.size());
    for (int u = 0; u < (int)result.size(); u++) {
        for (int v : left_moves[u])
            result[u].left_starts |= result[v].right_starts;
        for (int v : right_moves[u])
            result[u].right_starts &= result[v].left_starts;
    }
    return result;
}
