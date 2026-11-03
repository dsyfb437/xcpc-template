/**
 * @brief 零和游戏（最大化分差）
 * @complexity O(n+m)
 * @index 后继编号必须小于当前状态
 * @idea best[u] 按当前玩家的视角记分差。走一步拿到 reward，接着换对方走，所以这一步的结果是 reward - best[v]。
 * @usage auto best = zero_sum_scores(moves);
 *   moves[u] 中每项是 {下一个状态, 这一步拿到的分}。
 * @pitfall 终点分差为 0，奖励允许为负。中间结果须在 long long 范围内。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> zero_sum_scores(const vector<vector<pair<int, ll>>> &moves) {
    vector<ll> best(moves.size());
    for (int u = 0; u < (int)moves.size(); u++) {
        if (moves[u].empty())
            continue;
        best[u] = numeric_limits<ll>::lowest();
        for (auto [v, reward] : moves[u])
            best[u] = max(best[u], reward - best[v]);
    }
    return best;
}
