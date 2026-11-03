/**
 * @brief 全局最小割（Stoer–Wagner）
 * @complexity O(n^3)
 * @index 点 0-based
 * @idea 每轮按与已选点集的连边总权从大到小加点，最后一个点的割值是本轮候选，再把最后两点合并。
 * @usage auto cut = stoer_wagner(w); w 是对称容量矩阵，重边容量相加，无边填 0。
 * @pitfall 只适用于无向非负权图，容量累加须放进 long long。少于两个点时返回 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll stoer_wagner(vector<vector<ll>> w) {
    int n = (int)w.size();
    if (n < 2)
        return 0;
    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 0);
    ll answer = numeric_limits<ll>::max();

    while (vertices.size() > 1) {
        vector<ll> gain(n);
        vector<bool> used(n);
        int previous = -1;
        for (int i = 0; i < (int)vertices.size(); i++) {
            int u = -1;
            for (int v : vertices)
                if (!used[v] && (u == -1 || gain[v] > gain[u]))
                    u = v;
            used[u] = true;
            if (i + 1 == (int)vertices.size()) {
                answer = min(answer, gain[u]);
                for (int v : vertices)
                    if (v != u && v != previous) {
                        w[previous][v] += w[u][v];
                        w[v][previous] = w[previous][v];
                    }
                vertices.erase(find(vertices.begin(), vertices.end(), u));
                break;
            }
            previous = u;
            for (int v : vertices)
                if (!used[v])
                    gain[v] += w[u][v];
        }
    }
    return answer;
}
