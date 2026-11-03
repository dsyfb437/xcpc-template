/**
 * @brief 稳定匹配（Gale–Shapley）
 * @complexity O(n^2)
 * @index 两边均 0-based
 * @idea 未匹配的左点按偏好提议。右点保留更喜欢的人，把被拒的一方放回队列。
 * @usage stable_matching(left, right) 返回每个左点匹配的右点。偏好列表从最喜欢到最不喜欢。
 * @pitfall 两边人数相同，偏好严格且完整，每行都是 0 到 n - 1 的排列。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> stable_matching(const vector<vector<int>> &left,
                                   const vector<vector<int>> &right) {
    int n = (int)left.size();
    vector rank(n, vector<int>(n));
    for (int v = 0; v < n; v++)
        for (int i = 0; i < n; i++)
            rank[v][right[v][i]] = i;

    vector<int> next(n), partner(n, -1), answer(n, -1);
    queue<int> free;
    for (int u = 0; u < n; u++)
        free.push(u);
    while (!free.empty()) {
        int u = free.front();
        free.pop();
        int v = left[u][next[u]++];
        int old = partner[v];
        if (old != -1 && rank[v][old] < rank[v][u]) {
            free.push(u);
            continue;
        }
        if (old != -1)
            free.push(old);
        partner[v] = u;
        answer[u] = v;
    }
    return answer;
}
