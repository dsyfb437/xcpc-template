/**
 * @brief 树哈希（有根树）
 * @complexity O(n)
 * @index 点 1-based
 * @idea 把每个儿子的哈希混合后相加，儿子的排列顺序不影响结果。
 * @usage tree_hash(g, root, seed) 返回每个点的子树哈希，用同一个 seed 比较。
 * @pitfall g 是非空无向树。有哈希碰撞概率，要精确比较时用 AHU。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline vector<ull> tree_hash(const vector<vector<int>> &g, int root, ull seed) {
    auto mix = [](ull x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    };
    vector<int> parent(g.size()), order{root};
    for (int i = 0; i < (int)order.size(); i++) {
        int u = order[i];
        for (int v : g[u])
            if (v != parent[u]) {
                parent[v] = u;
                order.push_back(v);
            }
    }
    vector<ull> hash(g.size(), 1);
    for (int i = (int)order.size() - 1; i > 0; i--) {
        int u = order[i];
        hash[parent[u]] += mix(hash[u] + seed);
    }
    return hash;
}
