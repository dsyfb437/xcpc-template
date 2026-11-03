/**
 * @brief 爬山算法（最小化）
 * @complexity 每轮遍历一次邻域，最多 max_steps 轮
 * @index -
 * @idea 每轮选代价最小的邻居。走一步不能严格变好就停。
 * @usage auto best = hill_climb(start, 1000, neighbors, cost);
 *   neighbors(state) 返回邻居，cost(state) 越小越好。
 * @pitfall 可能卡在局部最优或平台。邻居太多时，可以只给一批候选。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class State, class Neighbors, class Cost>
State hill_climb(State current, int max_steps, Neighbors neighbors, Cost cost) {
    auto current_cost = cost(as_const(current));
    for (int step = 0; step < max_steps; step++) {
        State best = current;
        auto best_cost = current_cost;
        for (const auto &candidate : neighbors(as_const(current))) {
            auto value = cost(candidate);
            if (value < best_cost) {
                best = candidate;
                best_cost = value;
            }
        }
        if (!(best_cost < current_cost))
            break;
        current = move(best);
        current_cost = best_cost;
    }
    return current;
}
