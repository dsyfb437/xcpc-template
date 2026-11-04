/**
 * @brief 2-SAT
 * @complexity O(n + m)
 * @index 变量 1-based
 *
 * 变量 1..n；结点 i 表示 x_i 为真，i+n 表示 x_i 为假。
 * 用法：`TwoSAT sat(n); sat.add_clause(a, va, b, vb);` 表示 (x_a = va) 或 (x_b = vb)。
 *       `auto ans = sat.solve();` 空表示无解，否则 ans[i] 是 x_i 的取值。
 * 自带 Tarjan SCC；SCC 编号是反拓扑序，取 comp[i] < comp[i+n] 为真。
 *
 * 例：
 *   TwoSAT sat(n);
 *   sat.add_clause(a, true, b, false);
 *   vector<char> ans = sat.solve(); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct TarjanSCC {
    int n, cnt = 0, timer = 0;
    vector<int> comp, dfn, low, stk;
    vector<char> in_stk;

    explicit TarjanSCC(const vector<vector<int>> &g)
        : n((int)g.size() - 1), comp(n + 1, -1), dfn(n + 1), low(n + 1),
          in_stk(n + 1, 0) {
        for (int u = 1; u <= n; u++)
            if (!dfn[u])
                dfs(u, g);
    }

    void dfs(int u, const vector<vector<int>> &g) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in_stk[u] = 1;
        vector<pair<int, int>> frames{{u, 0}};
        while (!frames.empty()) {
            auto &[x, next] = frames.back();
            if (next < (int)g[x].size()) {
                int v = g[x][next++];
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++timer;
                    stk.push_back(v);
                    in_stk[v] = 1;
                    frames.emplace_back(v, 0);
                } else if (in_stk[v]) {
                    low[x] = min(low[x], dfn[v]);
                }
                continue;
            }
            int finished = x;
            frames.pop_back();
            if (!frames.empty()) {
                int parent = frames.back().first;
                low[parent] = min(low[parent], low[finished]);
            }
            if (low[finished] != dfn[finished]) continue;
            while (true) {
                int v = stk.back();
                stk.pop_back();
                in_stk[v] = 0;
                comp[v] = cnt;
                if (v == finished) break;
            }
            cnt++;
        }
    }

};

struct TwoSAT {
    int n;
    vector<vector<int>> g;

    explicit TwoSAT(int n) : n(n), g(2 * n + 1) {}

    int node(int i, bool v) const { return v ? i : i + n; }

    void add_clause(int a, bool va, int b, bool vb) {
        g[node(a, !va)].push_back(node(b, vb));
        g[node(b, !vb)].push_back(node(a, va));
    }

    // 返回每个变量的取值；无解返回空 vector。
    vector<char> solve() const {
        TarjanSCC scc(g);
        vector<char> ans(n + 1);
        for (int i = 1; i <= n; i++) {
            if (scc.comp[i] == scc.comp[i + n])
                return {};
            ans[i] = scc.comp[i] < scc.comp[i + n];
        }
        return ans;
    }
};
