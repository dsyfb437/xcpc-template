/**
 * @brief 最大团搜索（Bron–Kerbosch）
 * @complexity 最坏 O(n 3^(n/3))
 * @index 点 0-based
 * @idea 候选点只留与当前团全相邻的点。选一个枢轴，只分支枚举候选里不与它相邻的点。
 * @usage maximum_clique(adj) 返回最大团的点表，adj 是 0/1 邻接矩阵。
 * @pitfall 简单无向图，最多 64 个点，稠密大图仍可能很慢。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline vector<int> maximum_clique(const vector<vector<int>> &adj) {
    int n = (int)adj.size();
    vector<ull> neighbors(n);
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++)
            if (u != v && adj[u][v])
                neighbors[u] |= ull{1} << v;

    vector<int> best, clique;
    auto dfs = [&](auto &&self, ull available, ull used) -> void {
        if (clique.size() + popcount(available) <= best.size())
            return;
        if (!available) {
            best = clique;
            return;
        }

        int pivot = countr_zero(available | used);
        for (ull bits = available | used; bits; bits &= bits - 1) {
            int u = countr_zero(bits);
            if (popcount(neighbors[u] & available) >
                popcount(neighbors[pivot] & available))
                pivot = u;
        }
        ull choices = available & ~neighbors[pivot];
        for (; choices; choices &= choices - 1) {
            int u = countr_zero(choices);
            ull bit = ull{1} << u;
            clique.push_back(u);
            self(self, available & neighbors[u], used & neighbors[u]);
            clique.pop_back();
            available ^= bit;
            used |= bit;
        }
    };
    ull all = n == 64 ? ~ull{0} : (ull{1} << n) - 1;
    dfs(dfs, all, 0);
    return best;
}
