/**
 * @brief 虚树
 * @complexity O(k log k + kT)，k 是关键点个数，T 是单次 LCA 的时间
 * @index 点 1-based，tin 是 DFS 首次到达的顺序
 *
 * @idea 按 DFS 序排序，补上相邻关键点的 LCA。
 *       再按 DFS 序走一遍，用栈找到每个点最近的虚树祖先。
 * @usage get_lca(u, v) 返回 u、v 的 LCA。
 *        `auto [root, edges] = virtual_tree(keys, tin, get_lca);`
 *        edges 中每条边是 {父亲, 儿子}，沿用原树点编号。
 * @pitfall tin 和 get_lca 必须用同一个原树根。keys 可重复。
 *          空集返回根 0 和空边集。要保留原树根时，把它也放进 keys。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class GetLCA>
pair<int, vector<pair<int, int>>>
virtual_tree(vector<int> keys, const vector<int> &tin, GetLCA get_lca) {
    if (keys.empty())
        return {0, {}};
    auto by_tin = [&](int u, int v) { return tin[u] < tin[v]; };
    sort(keys.begin(), keys.end(), by_tin);
    int k = (int)keys.size();
    for (int i = 1; i < k; i++)
        keys.push_back(get_lca(keys[i - 1], keys[i]));

    sort(keys.begin(), keys.end(), by_tin);
    keys.erase(unique(keys.begin(), keys.end()), keys.end());
    vector<int> path;
    vector<pair<int, int>> edges;
    for (int u : keys) {
        while (!path.empty() && get_lca(path.back(), u) != path.back())
            path.pop_back();
        if (!path.empty())
            edges.emplace_back(path.back(), u);
        path.push_back(u);
    }
    return {keys.front(), move(edges)};
}
