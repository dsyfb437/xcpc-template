/**
 * @brief 平面图判定（Left–Right）
 * @complexity O(n + m log(m + 1))
 * @index 点和边编号 0-based
 * @idea 给 DFS 边定向并算返祖边的最低点。按嵌套深度处理出边，用冲突对栈检查左右约束能否同时满足。
 * @usage is_planar(n, edges) 返回图是否可平面嵌入。
 * @pitfall 输入是简单无向图，可不连通。判定使用递归 DFS。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline bool is_planar(int n, const vector<pair<int, int>> &edges) {
    int m = (int)edges.size();
    if (n >= 3 && m > 3LL * n - 6)
        return false;
    vector<vector<int>> incident(n), out(n);
    for (int e = 0; e < m; e++) {
        auto [u, v] = edges[e];
        incident[u].push_back(e);
        incident[v].push_back(e);
    }
    vector<int> height(n, -1), parent(n, -1), to(m, -1), roots;
    vector<int> low(m), second(m), nesting(m);
    auto orient = [&](auto &&self, int u) -> void {
        for (int e : incident[u]) {
            if (to[e] != -1)
                continue;
            auto [a, b] = edges[e];
            int v = a == u ? b : a;
            to[e] = v;
            out[u].push_back(e);
            low[e] = second[e] = height[u];
            if (height[v] == -1) {
                parent[v] = e;
                height[v] = height[u] + 1;
                self(self, v);
            } else {
                low[e] = height[v];
            }
            nesting[e] = 2 * low[e] + (second[e] < height[u]);
            int p = parent[u];
            if (p == -1)
                continue;
            if (low[e] < low[p]) {
                second[p] = min(low[p], second[e]);
                low[p] = low[e];
            } else if (low[e] > low[p]) {
                second[p] = min(second[p], low[e]);
            } else {
                second[p] = min(second[p], second[e]);
            }
        }
    };
    for (int u = 0; u < n; u++)
        if (height[u] == -1) {
            roots.push_back(u);
            height[u] = 0;
            orient(orient, u);
        }
    for (auto &list : out)
        sort(list.begin(), list.end(),
             [&](int a, int b) { return nesting[a] < nesting[b]; });

    struct Interval {
        int low_edge = -1, high_edge = -1;
        bool empty() const { return low_edge == -1; }
    };
    struct Conflict {
        Interval left, right;
    };
    vector<Conflict> stack;
    vector<int> ref(m, -1), first_return(m, -1);
    auto conflicts = [&](Interval a, int e) {
        return !a.empty() && low[a.high_edge] > low[e];
    };
    auto join = [&](Interval &a, Interval b) {
        if (b.empty())
            return;
        if (a.empty())
            a.high_edge = b.high_edge;
        else
            ref[a.low_edge] = b.high_edge;
        a.low_edge = b.low_edge;
    };

    auto merge_constraints = [&](int e, int p, int bottom) {
        Conflict merged;
        // 当前分支的返祖边先并到同一侧。
        while ((int)stack.size() > bottom) {
            Conflict top = stack.back();
            stack.pop_back();
            if (!top.left.empty())
                swap(top.left, top.right);
            if (!top.left.empty())
                return false;
            if (low[top.right.low_edge] > low[p])
                join(merged.right, top.right);
            else
                ref[top.right.low_edge] = first_return[p];
        }
        // 之前分支中与 e 冲突的区间必须放在另一侧。
        while (!stack.empty() && (conflicts(stack.back().left, e) ||
                                  conflicts(stack.back().right, e))) {
            Conflict top = stack.back();
            stack.pop_back();
            if (conflicts(top.right, e))
                swap(top.left, top.right);
            if (conflicts(top.right, e))
                return false;
            join(merged.right, top.right);
            join(merged.left, top.left);
        }
        if (!merged.left.empty() || !merged.right.empty())
            stack.push_back(merged);
        return true;
    };

    auto trim = [&](int p, int u) {
        auto lowest = [&](Conflict pair) {
            int a = pair.left.low_edge, b = pair.right.low_edge;
            if (a == -1)
                return low[b];
            if (b == -1)
                return low[a];
            return min(low[a], low[b]);
        };
        while (!stack.empty() && lowest(stack.back()) == height[u])
            stack.pop_back();
        if (!stack.empty()) {
            Conflict top = stack.back();
            stack.pop_back();
            auto drop = [&](Interval &a, Interval b) {
                while (a.high_edge != -1 && to[a.high_edge] == u)
                    a.high_edge = ref[a.high_edge];
                if (a.high_edge == -1 && !a.empty()) {
                    ref[a.low_edge] = b.low_edge;
                    a.low_edge = -1;
                }
            };
            drop(top.left, top.right);
            drop(top.right, top.left);
            if (!top.left.empty() || !top.right.empty())
                stack.push_back(top);
        }
        if (low[p] < height[u]) {
            int a = stack.back().left.high_edge;
            int b = stack.back().right.high_edge;
            ref[p] = a != -1 && (b == -1 || low[a] > low[b]) ? a : b;
        }
    };

    auto check = [&](auto &&self, int u) -> bool {
        int p = parent[u];
        for (int e : out[u]) {
            int v = to[e], bottom = (int)stack.size();
            if (parent[v] == e) {
                if (!self(self, v))
                    return false;
            } else {
                first_return[e] = e;
                stack.push_back({{}, {e, e}});
            }
            if (low[e] >= height[u])
                continue;
            if (e == out[u].front())
                first_return[p] = first_return[e];
            else if (!merge_constraints(e, p, bottom))
                return false;
        }
        if (p != -1) {
            auto [a, b] = edges[p];
            trim(p, a == u ? b : a);
        }
        return true;
    };
    for (int u : roots)
        if (!check(check, u))
            return false;
    return true;
}
