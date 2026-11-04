/**
 * @brief 支配树（Lengauer–Tarjan）
 * @complexity O((n + m) log n)
 * @index 点 0-based
 * @idea 按 DFS 编号从大到小求半支配点。带最小值的祖先压缩用于 eval，最后把候选父亲修正成直接支配点。
 * @usage dominator_tree(g, root) 返回每个点的直接支配点。
 * @pitfall 输入非空有向图，根和不可达点的答案是 -1。DFS 和路径压缩都使用显式栈。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> dominator_tree(const vector<vector<int>> &g, int root) {
    int n = (int)g.size();
    vector<int> number(n), vertex{0}, parent{0};
    number[root] = 1;
    vertex.push_back(root);
    parent.push_back(0);
    vector<pair<int, int>> frames{{root, 0}};
    while (!frames.empty()) {
        auto &[u, next] = frames.back();
        if (next == (int)g[u].size()) {
            frames.pop_back();
            continue;
        }
        int v = g[u][next++];
        if (number[v]) continue;
        number[v] = (int)vertex.size();
        vertex.push_back(v);
        parent.push_back(number[u]);
        frames.emplace_back(v, 0);
    }
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
    vector<int> path;
    auto eval = [&](int u) {
        path.clear();
        for (int x = u; ancestor[ancestor[x]]; x = ancestor[x])
            path.push_back(x);
        for (int i = (int)path.size() - 1; i >= 0; i--) {
            int x = path[i], p = ancestor[x];
            if (semi[label[p]] < semi[label[x]]) label[x] = label[p];
            ancestor[x] = ancestor[p];
        }
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
