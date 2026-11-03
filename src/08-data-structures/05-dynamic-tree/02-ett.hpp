/**
 * @brief Euler Tour Tree（动态连通与分量和）
 * @complexity 单次期望 O(log n)
 * @index 点 1-based，节点编号 0 表示空
 * @idea 每个点保留一个标记，每条边放两个方向的标记。隐式 Treap 存环形游览序，连边拼两个环，断边在两条方向标记处拆开。
 * @usage EulerTourTree t(n,seed); t.link(u,v); t.cut(u,v); t.set(u,x); t.component(u) 返回 {点数,权和}。
 * @pitfall 图必须是森林，非法 link 或 cut 返回 false。点权初始为 0，分量和用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct EulerTourTree {
    struct Node {
        ull priority = 0;
        int left = 0, right = 0, parent = 0, size = 0, vertices = 0;
        bool vertex = false;
        ll value = 0;
        __int128 sum = 0;
    };
    vector<Node> nodes{Node{}};
    map<pair<int, int>, pair<int, int>> edge;
    vector<int> recycled;
    mt19937_64 rng;

    explicit EulerTourTree(int n, ull seed = random_device{}()) : rng(seed) {
        for (int u = 1; u <= n; u++)
            make(true);
    }

    int make(bool vertex) {
        Node node{rng(), 0, 0, 0, 1, (int)vertex, vertex};
        if (!recycled.empty()) {
            int id = recycled.back();
            recycled.pop_back();
            nodes[id] = node;
            return id;
        }
        nodes.push_back(node);
        return (int)nodes.size() - 1;
    }

    void pull(int u) {
        auto &a = nodes[u];
        auto &l = nodes[a.left], &r = nodes[a.right];
        a.size = 1 + l.size + r.size;
        a.vertices = a.vertex + l.vertices + r.vertices;
        a.sum = a.value + l.sum + r.sum;
        if (a.left)
            l.parent = u;
        if (a.right)
            r.parent = u;
    }

    int merge(int a, int b) {
        if (!a || !b) {
            int u = a ? a : b;
            nodes[u].parent = 0;
            return u;
        }
        if (nodes[a].priority >= nodes[b].priority) {
            nodes[a].right = merge(nodes[a].right, b);
            pull(a);
            nodes[a].parent = 0;
            return a;
        }
        nodes[b].left = merge(a, nodes[b].left);
        pull(b);
        nodes[b].parent = 0;
        return b;
    }

    pair<int, int> split(int u, int count) {
        if (!u)
            return {0, 0};
        int left = nodes[nodes[u].left].size;
        if (count <= left) {
            auto [a, b] = split(nodes[u].left, count);
            nodes[u].left = b;
            pull(u);
            nodes[u].parent = 0;
            return {a, u};
        }
        auto [a, b] = split(nodes[u].right, count - left - 1);
        nodes[u].right = a;
        pull(u);
        nodes[u].parent = 0;
        return {u, b};
    }

    int root(int u) const {
        while (nodes[u].parent)
            u = nodes[u].parent;
        return u;
    }

    int index(int u) const {
        int pos = nodes[nodes[u].left].size;
        for (int p = nodes[u].parent; p; u = p, p = nodes[u].parent)
            if (nodes[p].right == u)
                pos += nodes[nodes[p].left].size + 1;
        return pos;
    }

    int reroot(int u) {
        auto [a, b] = split(root(u), index(u));
        return merge(b, a);
    }

    bool connected(int u, int v) const { return root(u) == root(v); }

    bool link(int u, int v) {
        if (connected(u, v))
            return false;
        int a = reroot(u), b = reroot(v);
        int first = make(false), second = make(false);
        merge(merge(merge(a, first), b), second);
        edge[minmax(u, v)] = {first, second};
        return true;
    }

    bool cut(int u, int v) {
        auto it = edge.find(minmax(u, v));
        if (it == edge.end())
            return false;
        auto [first, second] = it->second;
        int a = index(first), b = index(second);
        if (a > b)
            swap(a, b);
        auto [before, tail] = split(root(first), a);
        auto [first_edge, rest] = split(tail, 1);
        auto [middle, after_middle] = split(rest, b - a - 1);
        auto [second_edge, after] = split(after_middle, 1);
        merge(before, after);
        recycled.push_back(first_edge);
        recycled.push_back(second_edge);
        edge.erase(it);
        return true;
    }

    void set(int u, ll value) {
        nodes[u].value = value;
        for (; u; u = nodes[u].parent)
            pull(u);
    }

    pair<int, __int128> component(int u) const {
        const auto &a = nodes[root(u)];
        return {a.vertices, a.sum};
    }
};
