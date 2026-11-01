/**
 * @brief 2-SAT
 * @complexity O(n + m)
 * @index 变量 1-based
 *
 * 变量 1..n；结点 i 表示 x_i 为真，i+n 表示 x_i 为假。
 * 用法：`TwoSAT sat(n); sat.add_clause(a, va, b, vb);` 表示 (x_a = va) 或 (x_b = vb)。
 *       `auto ans = sat.solve();` 空表示无解，否则 ans[i] 是 x_i 的取值。
 * 依赖 tarjan_scc.hpp；SCC 编号是反拓扑序，取 comp[i] < comp[i+n] 为真。
 *
 * 例：
 *   TwoSAT sat(n);
 *   sat.add_clause(a, true, b, false);
 *   vector<char> ans = sat.solve(); */
#pragma once
#include <bits/stdc++.h>
#include "02-强连通分量.hpp"
using namespace std;

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
