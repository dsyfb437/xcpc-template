/**
 * @brief 有限状态自动机（DFA）
 * @complexity 建表 O(状态数 × 字符数)，匹配 O(串长)
 * @index 状态从 0 开始，next[u][c] 的 c 是字符编号
 * @idea 每读一个字符就按表转移。缺失的转移立即拒绝，读完后看当前状态是不是接受态。
 * @usage FiniteAutomaton<26> dfa(2);
 *   dfa.next[0][0] = 1;  // 从状态 0 读到 a，走到状态 1
 *   dfa.accept[1] = true;
 *   bool ok = dfa.accepts("a");
 * @pitfall 默认从状态 0 开始，字符从 a 连续编号。需要别的起点或字符范围时传给 accepts。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <size_t Alphabet = 26> struct FiniteAutomaton {
    vector<array<int, Alphabet>> next;
    vector<bool> accept;

    explicit FiniteAutomaton(int states) : next(states), accept(states) {
        for (auto &row : next)
            row.fill(-1);
    }

    bool accepts(string_view text, int start = 0, unsigned char first = 'a') const {
        int u = start;
        if (u < 0 || (size_t)u >= next.size())
            return false;
        for (unsigned char ch : text) {
            int c = ch - first;
            if (c < 0 || (size_t)c >= Alphabet)
                return false;
            u = next[u][c];
            if (u < 0 || (size_t)u >= next.size())
                return false;
        }
        return accept[u];
    }
};
