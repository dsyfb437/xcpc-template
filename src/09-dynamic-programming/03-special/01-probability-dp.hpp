/**
 * @brief 期望 DP（带自环的拓扑状态）
 * @complexity O(n + m)
 * @index 状态 0-based，非自环转移只去更大编号
 * @idea E[u] = 1 + sum(p E[v])。把自环项移到左边，再除以单步离开当前状态的概率。
 * @usage expected_absorption(transitions) 返回各状态到终止状态的期望步数，转移为 {v,p}。
 * @pitfall 空转移表是终止状态。非终止状态概率和为 1，概率非负。无法保证最终终止时返回无穷大。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

inline vector<ld>
expected_absorption(const vector<vector<pair<int, ld>>> &transitions) {
    int n = (int)transitions.size();
    vector<ld> expected(n);
    for (int u = n - 1; u >= 0; u--) {
        if (transitions[u].empty())
            continue;
        ld leave = 0, total = 1;
        for (auto [v, probability] : transitions[u]) {
            if (v == u || probability == 0)
                continue;
            leave += probability;
            total += probability * expected[v];
        }
        expected[u] = leave ? total / leave : numeric_limits<ld>::infinity();
    }
    return expected;
}
