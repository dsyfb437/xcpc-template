/**
 * @brief 一般图最大匹配（带花树）
 * @complexity O(n^3)
 * @index 点 0-based
 * @idea 用 BFS 找交错增广路。遇到两个外点之间的边，就缩掉形成的奇环后继续找。
 * @usage auto mate = blossom(g); mate[u] 是配对点，-1 表示未匹配。
 * @pitfall 输入为无向图，不带自环。匹配条数是已匹配点数除以 2。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> blossom(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<int> mate(n, -1), parent(n), base(n);
    for (int root = 0; root < n; root++) {
        if (mate[root] != -1)
            continue;

        fill(parent.begin(), parent.end(), -1);
        iota(base.begin(), base.end(), 0);
        vector<bool> outer(n);
        queue<int> q;
        outer[root] = true;
        q.push(root);

        auto common_base = [&](int a, int b) {
            vector<bool> seen(n);
            while (true) {
                a = base[a];
                seen[a] = true;
                if (mate[a] == -1)
                    break;
                a = parent[mate[a]];
            }
            while (!seen[base[b]])
                b = parent[mate[base[b]]];
            return base[b];
        };

        int end = -1;
        while (!q.empty() && end == -1) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (base[u] == base[v] || mate[u] == v)
                    continue;
                if (outer[v]) {
                    int center = common_base(u, v);
                    vector<bool> in_flower(n);
                    auto mark = [&](int x, int from) {
                        while (base[x] != center) {
                            int y = mate[x];
                            in_flower[base[x]] = in_flower[base[y]] = true;
                            parent[x] = from;
                            from = y;
                            x = parent[y];
                        }
                    };
                    mark(u, v);
                    mark(v, u);
                    for (int x = 0; x < n; x++)
                        if (in_flower[base[x]]) {
                            base[x] = center;
                            if (!outer[x]) {
                                outer[x] = true;
                                q.push(x);
                            }
                        }
                } else if (parent[v] == -1) {
                    parent[v] = u;
                    if (mate[v] == -1) {
                        end = v;
                        break;
                    }
                    outer[mate[v]] = true;
                    q.push(mate[v]);
                }
            }
        }

        while (end != -1) {
            int u = parent[end], next = mate[u];
            mate[u] = end;
            mate[end] = u;
            end = next;
        }
    }
    return mate;
}
