/**
 * @brief 动态 DP（树上最大权独立集）
 * @complexity 预处理 O(n)，修改 O(log^2 n)，取答案 O(log n)
 * @index 树点 1-based
 * @idea 轻儿子的贡献收进点矩阵。重链上做 max-plus 矩阵乘积，点权变化后只沿轻边向上传差值。
 * @usage DynamicIndependentSet t(g,weight,root); t.set(u,x); t.answer()。
 * @pitfall 树拓扑固定、初始非空，weight[0] 留空。允许负点权和空集，答案用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class DynamicIndependentSet {
    using Wide = __int128;
    static constexpr Wide minus_inf = -((Wide)1 << 120);
    using Matrix = array<array<Wide, 2>, 2>;
    int root, base;
    vector<int> parent, head, position, end;
    vector<ll> weight;
    vector<Matrix> local, segment;

    static Matrix identity() { return {{{0, minus_inf}, {minus_inf, 0}}}; }

    static Matrix multiply(const Matrix &a, const Matrix &b) {
        Matrix c{{{minus_inf, minus_inf}, {minus_inf, minus_inf}}};
        for (int i = 0; i < 2; i++)
            for (int k = 0; k < 2; k++)
                if (a[i][k] != minus_inf)
                    for (int j = 0; j < 2; j++)
                        if (b[k][j] != minus_inf)
                            c[i][j] = max(c[i][j], a[i][k] + b[k][j]);
        return c;
    }

    array<Wide, 2> chain(int top) const {
        Matrix left = identity(), right = identity();
        for (int l = position[top] + base, r = end[top] + base; l < r;
             l /= 2, r /= 2) {
            if (l & 1)
                left = multiply(left, segment[l++]);
            if (r & 1)
                right = multiply(segment[--r], right);
        }
        auto result = multiply(left, right);
        return {result[0][0], result[1][0]};
    }

    void refresh(int u) {
        int pos = base + position[u];
        segment[pos] = local[u];
        while (pos > 1) {
            pos /= 2;
            segment[pos] = multiply(segment[pos * 2], segment[pos * 2 + 1]);
        }
    }

  public:
    DynamicIndependentSet(const vector<vector<int>> &g, const vector<ll> &weight,
                          int root = 1)
        : root(root), base((int)bit_ceil((unsigned)g.size() - 1)), parent(g.size()),
          head(g.size()), position(g.size()), end(g.size()), weight(weight),
          local(g.size()), segment(2 * base, identity()) {
        vector<int> order{root}, size(g.size(), 1), heavy(g.size());
        size[0] = 0;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : g[u])
                if (v != parent[u]) {
                    parent[v] = u;
                    order.push_back(v);
                }
        }
        vector<array<Wide, 2>> dp(g.size());
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i], p = parent[u];
            auto &[no, yes] = dp[u];
            yes += weight[u];
            int v = heavy[u];
            Wide a = no - max(dp[v][0], dp[v][1]);
            Wide b = yes - dp[v][0];
            local[u] = {{{a, a}, {b, minus_inf}}};
            if (p) {
                size[p] += size[u];
                if (size[u] > size[heavy[p]])
                    heavy[p] = u;
                dp[p][0] += max(no, yes);
                dp[p][1] += no;
            }
        }

        vector<int> pending{root};
        int timer = 0;
        while (!pending.empty()) {
            int top = pending.back();
            pending.pop_back();
            for (int u = top; u; u = heavy[u]) {
                head[u] = top;
                position[u] = timer++;
                segment[base + position[u]] = local[u];
                for (int v : g[u])
                    if (v != parent[u] && v != heavy[u])
                        pending.push_back(v);
            }
            end[top] = timer;
        }
        for (int u = base - 1; u > 0; u--)
            segment[u] = multiply(segment[u * 2], segment[u * 2 + 1]);
    }

    void set(int u, ll value) {
        local[u][1][0] += (Wide)value - weight[u];
        weight[u] = value;
        while (u) {
            int top = head[u];
            auto [old_no, old_yes] = chain(top);
            refresh(u);
            int p = parent[top];
            if (!p)
                break;
            auto [no, yes] = chain(top);
            Wide delta = max(no, yes) - max(old_no, old_yes);
            local[p][0][0] += delta;
            local[p][0][1] += delta;
            local[p][1][0] += no - old_no;
            u = p;
        }
    }

    Wide answer() const {
        auto [no, yes] = chain(root);
        return max(no, yes);
    }
};
