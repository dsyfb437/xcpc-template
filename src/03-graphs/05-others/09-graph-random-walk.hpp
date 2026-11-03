/**
 * @brief 图上随机游走（首次到达期望）
 * @complexity O(n^3 + m)
 * @index 点 0-based
 * @idea 目标点期望为 0。其他点满足 deg[u] E[u] - sum E[v] = deg[u]，列方程后做高斯消元。
 * @usage auto e = expected_graph_walk(g, target); e[u] 是 u 首次到目标的期望步数。
 * @pitfall 无向无权图，每步等概率选一条邻边。无法到达目标时返回 -1。浮点实现适合小图。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

inline vector<ld> expected_graph_walk(const vector<vector<int>> &g, int target) {
    int n = (int)g.size();
    vector<int> id(n, -1), order{target};
    id[target] = 0;
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : g[order[i]])
            if (id[v] == -1) {
                id[v] = (int)order.size();
                order.push_back(v);
            }

    int k = (int)order.size();
    vector a(k, vector<ld>(k + 1));
    a[0][0] = 1;
    for (int i = 1; i < k; i++) {
        int u = order[i];
        a[i][i] = a[i][k] = g[u].size();
        for (int v : g[u])
            a[i][id[v]]--;
    }

    for (int i = 0; i < k; i++) {
        int pivot = i;
        for (int j = i + 1; j < k; j++)
            if (abs(a[j][i]) > abs(a[pivot][i]))
                pivot = j;
        swap(a[i], a[pivot]);
        ld div = a[i][i];
        for (int j = i; j <= k; j++)
            a[i][j] /= div;
        for (int j = 0; j < k; j++) {
            if (j == i)
                continue;
            ld factor = a[j][i];
            for (int c = i; c <= k; c++)
                a[j][c] -= factor * a[i][c];
        }
    }

    vector<ld> answer(n, -1);
    for (int i = 0; i < k; i++)
        answer[order[i]] = a[i][k];
    return answer;
}
