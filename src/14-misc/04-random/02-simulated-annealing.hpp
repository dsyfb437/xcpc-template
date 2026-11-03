/**
 * @brief 模拟退火（最小化）
 * @complexity O(steps) 次生成邻居和算分，另存常数个状态
 * @index -
 * @idea 更好的邻居直接接受，变差 Δ 时以 exp(-Δ/T) 的概率接受。温度按等比下降，最后返回见过的最好状态。
 * @usage auto best = simulated_annealing(start, 50000, 100.0, 0.01,
 *                                  rng, neighbor, cost);
 *   neighbor(state, rng) 生成新状态，cost(state) 越小越好。
 * @pitfall 温度必须为正，起始温度不小于结束温度。cost 要有限，结果随种子和参数变化。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

template <class State, class RNG, class Neighbor, class Cost>
State simulated_annealing(State current, int steps, double start_temp,
                          double end_temp, RNG &rng, Neighbor neighbor, Cost cost) {
    assert(isfinite(start_temp) && start_temp >= end_temp && end_temp > 0);
    State best = current;
    auto current_cost = cost(as_const(current)), best_cost = current_cost;
    double log_start = log(start_temp), log_end = log(end_temp);
    uniform_real_distribution<double> random01(0, 1);

    for (int step = 0; step < steps; step++) {
        double ratio = steps == 1 ? 0.0 : double(step) / (steps - 1);
        double temperature = exp(lerp(log_start, log_end, ratio));
        State candidate = neighbor(as_const(current), rng);
        auto candidate_cost = cost(as_const(candidate));
        ld increase = (ld)candidate_cost - current_cost;

        if (candidate_cost <= current_cost ||
            random01(rng) < exp(-increase / temperature)) {
            current = move(candidate);
            current_cost = candidate_cost;
            if (current_cost < best_cost) {
                best = current;
                best_cost = current_cost;
            }
        }
    }
    return best;
}
