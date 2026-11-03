/**
 * @brief Alpha-Beta 剪枝
 * @complexity 最坏遍历整棵博弈树，栈空间 O(树高)
 * @index 节点 0-based
 * @idea 最大方更新 alpha，最小方更新 beta。alpha ≥ beta 时，剩下的分支已经不能改变祖先的选择。
 * @usage long long score = alpha_beta(children, value, root);
 *   children[u] 为空时直接返回 value[u]。默认根节点是最大方。
 * @pitfall 双方轮流行动。子节点顺序影响剪枝效果，优先搜更可能被选中的走法。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll alpha_beta(const vector<vector<int>> &children, const vector<ll> &value,
                     int root, bool maximizing = true) {
    constexpr ll low = numeric_limits<ll>::lowest();
    constexpr ll high = numeric_limits<ll>::max();
    auto dfs = [&](auto &&self, int u, bool turn, ll alpha, ll beta) -> ll {
        if (children[u].empty())
            return value[u];
        ll best = turn ? low : high;
        for (int v : children[u]) {
            ll score = self(self, v, !turn, alpha, beta);
            if (turn) {
                best = max(best, score);
                alpha = max(alpha, best);
            } else {
                best = min(best, score);
                beta = min(beta, best);
            }
            if (alpha >= beta)
                break;
        }
        return best;
    };
    return dfs(dfs, root, maximizing, low, high);
}
