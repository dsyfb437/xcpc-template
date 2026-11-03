/**
 * @brief IDA*（单位步长）
 * @complexity 时间取决于估价，搜索栈深度不超过 max_depth
 * @index 状态支持相等比较
 * @idea 用 DFS 搜 g+h 不超过阈值的状态。失败后把阈值提到这轮超出的最小值。
 * @usage auto path = ida_star_path(start, goal, max_depth, neighbors, estimate);
 *   neighbors(s) 返回一步能到的状态，estimate(s) 返回剩余步数的下界。
 * @pitfall 每步代价为 1，估价非负且不能高估。步数上限内无解时返回空数组。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class State, class Neighbors, class Estimate>
vector<State> ida_star_path(State start, const State &goal, int max_depth,
                            Neighbors neighbors, Estimate estimate) {
    vector<State> path = {start};
    ll limit = estimate(start);
    while (limit <= max_depth) {
        ll next_limit = numeric_limits<ll>::max();
        auto dfs = [&](auto &&self, int depth) -> bool {
            State u = path.back();
            ll score = depth + (ll)estimate(u);
            if (score > limit) {
                next_limit = min(next_limit, score);
                return false;
            }
            if (u == goal)
                return true;
            if (depth == max_depth)
                return false;
            for (const auto &v : neighbors(u)) {
                if (find(path.begin(), path.end(), v) != path.end())
                    continue;
                path.push_back(v);
                if (self(self, depth + 1))
                    return true;
                path.pop_back();
            }
            return false;
        };
        if (dfs(dfs, 0))
            return path;
        limit = next_limit;
    }
    return {};
}
