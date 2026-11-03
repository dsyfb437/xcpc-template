/**
 * @brief AHU 树同构
 * @complexity O(n log n)，n 是两棵树的总点数
 * @index 点 1-based
 *
 * @idea 把儿子的类型编号排序后作为当前点的类型。无根树以中心为根，
 *       两个中心时分别比较。两棵树共用编号表，比较结果没有哈希碰撞。
 * @usage tree_isomorphic(g, h) 判断两棵无根树是否同构。
 * @pitfall 输入为无权无向树，空树用长度为 1 的邻接表表示。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline bool tree_isomorphic(const vector<vector<int>> &g,
                            const vector<vector<int>> &h) {
    if (g.size() != h.size())
        return false;
    if (g.size() <= 1)
        return true;

    auto centers = [](const vector<vector<int>> &tree) {
        int n = (int)tree.size() - 1;
        vector<int> deg(n + 1), leaves;
        for (int u = 1; u <= n; u++) {
            deg[u] = (int)tree[u].size();
            if (deg[u] <= 1)
                leaves.push_back(u);
        }
        int left = 0;
        while (n - left > 2) {
            int right = (int)leaves.size();
            while (left < right) {
                int u = leaves[left++];
                for (int v : tree[u])
                    if (--deg[v] == 1)
                        leaves.push_back(v);
            }
        }
        return vector<int>(leaves.begin() + left, leaves.end());
    };

    map<vector<int>, int> types;
    auto rooted_type = [&](const vector<vector<int>> &tree, int root) {
        vector<int> parent(tree.size()), type(tree.size()), order{root};
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : tree[u])
                if (v != parent[u]) {
                    parent[v] = u;
                    order.push_back(v);
                }
        }
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            vector<int> children;
            for (int v : tree[u])
                if (v != parent[u])
                    children.push_back(type[v]);
            sort(children.begin(), children.end());
            int next_id = (int)types.size() + 1;
            type[u] = types.try_emplace(children, next_id).first->second;
        }
        return type[root];
    };

    int target = rooted_type(g, centers(g).front());
    for (int root : centers(h))
        if (rooted_type(h, root) == target)
            return true;
    return false;
}
