/**
 * @brief 支配树（Lengauer–Tarjan）
 * @complexity O((n + m) log n)
 * @index 点 0-based
 * @idea 按 DFS 编号从大到小求半支配点。带最小值的祖先压缩用于 eval，最后把候选父亲修正成直接支配点。
 * @usage dominator_tree(g, root) 返回每个点的直接支配点。
 * @pitfall 输入有向图，根和不可达点的答案是 -1。DFS 和路径压缩使用递归。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> dominator_tree(const vector<vector<int>> &g, int root) {
    int n = (int)g.size();
    vector<int> number(n), vertex{0}, parent{0};
    auto dfs = [&](auto &&self, int u, int p) -> void {
        number[u] = (int)vertex.size();
        vertex.push_back(u);
        parent.push_back(p);
        for (int v : g[u])
            if (!number[v])
                self(self, v, number[u]);
    };
    dfs(dfs, root, 0);
    int count = (int)vertex.size() - 1;
    vector<vector<int>> from(count + 1), bucket(count + 1);
    for (int u = 0; u < n; u++)
        if (number[u])
            for (int v : g[u])
                from[number[v]].push_back(number[u]);

    vector<int> semi(count + 1), label(count + 1), ancestor(count + 1);
    vector<int> idom(count + 1);
    iota(semi.begin(), semi.end(), 0);
    iota(label.begin(), label.end(), 0);
    auto compress = [&](auto &&self, int u) -> void {
        int p = ancestor[u];
        if (!ancestor[p])
            return;
        self(self, p);
        if (semi[label[p]] < semi[label[u]])
            label[u] = label[p];
        ancestor[u] = ancestor[p];
    };
    auto eval = [&](int u) {
        compress(compress, u);
        return label[u];
    };

    for (int u = count; u > 1; u--) {
        for (int v : from[u])
            semi[u] = min(semi[u], semi[eval(v)]);
        bucket[semi[u]].push_back(u);
        int p = parent[u];
        ancestor[u] = p;
        for (int v : bucket[p]) {
            int w = eval(v);
            idom[v] = semi[w] < semi[v] ? w : p;
        }
        bucket[p].clear();
    }

    vector<int> answer(n, -1);
    for (int u = 2; u <= count; u++) {
        if (idom[u] != semi[u])
            idom[u] = idom[idom[u]];
        answer[vertex[u]] = vertex[idom[u]];
    }
    return answer;
}
