/**
 * @brief 平面图判定（Left–Right）
 * @complexity O(n + m log(m + 1))
 * @index 点和边编号 0-based
 * @idea 给 DFS 边定向并算返祖边的最低点。按嵌套深度处理出边，用冲突对栈检查左右约束能否同时满足。
 * @usage is_planar(n, edges) 返回图是否可平面嵌入。
 * @pitfall 输入是简单无向图，可不连通。两个 DFS 阶段都用显式栈，支持深链。
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
    auto finish_edge = [&](int u, int e) {
        nesting[e] = 2 * low[e] + (second[e] < height[u]);
        int p = parent[u];
        if (p == -1)
            return;
        if (low[e] < low[p]) {
            second[p] = min(low[p], second[e]);
            low[p] = low[e];
        } else if (low[e] > low[p]) {
            second[p] = min(second[p], low[e]);
        } else {
            second[p] = min(second[p], second[e]);
        }
    };
    for (int root = 0; root < n; root++) {
        if (height[root] != -1)
            continue;
        roots.push_back(root);
        height[root] = 0;
        vector<pair<int, int>> frames{{root, 0}};
        while (!frames.empty()) {
            auto &[u, next] = frames.back();
            if (next == (int)incident[u].size()) {
                int e = parent[u], child = u;
                frames.pop_back();
                if (e != -1) {
                    auto [a, b] = edges[e];
                    finish_edge(a == child ? b : a, e);
                }
                continue;
            }
            int e = incident[u][next++];
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
                frames.emplace_back(v, 0);
            } else {
                low[e] = height[v];
                finish_edge(u, e);
            }
        }
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

    struct Frame {
        int u, next = 0, pending = -1, bottom = 0;
    };
    for (int root : roots) {
        vector<Frame> frames{{root}};
        while (!frames.empty()) {
            auto &f = frames.back();
            int u = f.u, p = parent[u], e, bottom;
            if (f.pending != -1) {
                // 子树已处理完，回来合并这条树边的约束。
                e = f.pending;
                bottom = f.bottom;
                f.pending = -1;
            } else {
                if (f.next == (int)out[u].size()) {
                    if (p != -1) {
                        auto [a, b] = edges[p];
                        trim(p, a == u ? b : a);
                    }
                    frames.pop_back();
                    continue;
                }
                e = out[u][f.next++];
                bottom = (int)stack.size();
                int v = to[e];
                if (parent[v] == e) {
                    f.pending = e;
                    f.bottom = bottom;
                    frames.push_back({v});
                    continue;
                }
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
    }
    return true;
}
