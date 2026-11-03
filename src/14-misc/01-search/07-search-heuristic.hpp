/**
 * @brief 启发式搜索（束搜索）
 * @complexity 每层生成 K 个候选时，平均 O(K) 筛选时间和 O(K) 空间
 * @index 按步数逐层扩展
 * @idea 每层只留下估价最小的 width 个状态，再从它们继续搜。
 * @usage auto found = beam_search(start, width, max_depth, neighbors, goal, score);
 *   neighbors(s) 给下一层状态，goal(s) 判断完成，score(s) 越小越好。
 * @pitfall 估价相同的状态保留哪一个不固定。被丢掉的状态可能通向解，返回空不代表无解。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class State, class Neighbors, class Goal, class Score>
optional<State> beam_search(State start, size_t width, int max_depth,
                            Neighbors neighbors, Goal goal, Score score) {
    using Candidate = pair<decltype(score(start)), State>;
    if (max_depth < 0)
        return nullopt;
    if (goal(start))
        return start;
    if (width == 0)
        return nullopt;

    auto by_score = [](const Candidate &a, const Candidate &b) {
        return a.first < b.first;
    };
    vector<Candidate> layer;
    layer.emplace_back(score(start), move(start));
    for (int depth = 0; depth < max_depth && !layer.empty(); depth++) {
        vector<Candidate> next;
        for (const auto &entry : layer) {
            for (const auto &v : neighbors(entry.second)) {
                if (goal(v))
                    return v;
                next.emplace_back(score(v), v);
            }
        }
        if (next.size() > width) {
            auto cut = next.begin() + width;
            nth_element(next.begin(), cut, next.end(), by_score);
            next.erase(cut, next.end());
        }
        layer.swap(next);
    }
    return nullopt;
}
