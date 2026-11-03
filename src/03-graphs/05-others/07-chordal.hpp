/**
 * @brief 弦图判定（最大势搜索）
 * @complexity O(n + m)
 * @index 点 0-based
 * @idea 反复选已有邻点最多的点。把选择顺序倒过来，检查每个点的后继邻点是否成团。
 * @usage auto peo = chordal_order(g); 非空图返回空数组表示不是弦图，否则返回完美消除序。
 * @pitfall 输入为简单无向图，允许不连通。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> chordal_order(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<vector<int>> buckets(n + 1);
    vector<int> score(n), position(n, -1), order;
    for (int u = 0; u < n; u++)
        buckets[0].push_back(u);

    int highest = 0;
    for (int i = n - 1; i >= 0; i--) {
        while (true) {
            auto &bucket = buckets[highest];
            if (!bucket.empty()) {
                int u = bucket.back();
                bucket.pop_back();
                if (position[u] != -1 || score[u] != highest)
                    continue;
                position[u] = i;
                order.push_back(u);
                for (int v : g[u])
                    if (position[v] == -1) {
                        buckets[++score[v]].push_back(v);
                        highest = max(highest, score[v]);
                    }
                break;
            }
            highest--;
        }
    }

    vector<vector<int>> children(n);
    for (int u = 0; u < n; u++) {
        int p = -1;
        for (int v : g[u])
            if (position[v] > position[u] && (p == -1 || position[v] < position[p]))
                p = v;
        if (p != -1)
            children[p].push_back(u);
    }
    vector<int> adjacent(n, -1);
    for (int p = 0; p < n; p++) {
        for (int v : g[p])
            adjacent[v] = p;
        for (int u : children[p])
            for (int v : g[u])
                if (position[v] > position[u] && v != p && adjacent[v] != p)
                    return {};
    }
    reverse(order.begin(), order.end());
    return order;
}
