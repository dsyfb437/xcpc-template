/**
 * @brief Link-Cut Tree（动态树链异或）
 * @complexity 单次均摊 O(log n)
 * @index 点 1-based，0 表示空
 * @idea 用 Splay 存首选路径。access 换首选儿子，make_root 翻转暴露出的路径。
 * @usage LinkCutTree t(n); t.set(u,x); t.link(u,v); t.cut(u,v); t.path_xor(u,v)。
 * @pitfall link 只连接不同分量，cut 只断真实边，两者返回是否成功。不连通时路径查询返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct LinkCutTree {
    struct Node {
        int child[2]{}, parent = 0;
        ull value = 0, sum = 0;
        bool reverse = false;
    };
    vector<Node> nodes;

    explicit LinkCutTree(int n) : nodes(n + 1) {}

    bool aux_root(int u) const {
        int p = nodes[u].parent;
        return !p || (nodes[p].child[0] != u && nodes[p].child[1] != u);
    }

    void pull(int u) {
        auto &a = nodes[u];
        a.sum = a.value ^ nodes[a.child[0]].sum ^ nodes[a.child[1]].sum;
    }

    void flip(int u) {
        if (u) {
            swap(nodes[u].child[0], nodes[u].child[1]);
            nodes[u].reverse = !nodes[u].reverse;
        }
    }

    void push(int u) {
        if (nodes[u].reverse) {
            flip(nodes[u].child[0]);
            flip(nodes[u].child[1]);
            nodes[u].reverse = false;
        }
    }

    void rotate(int u) {
        int p = nodes[u].parent, g = nodes[p].parent;
        int side = nodes[p].child[1] == u;
        int middle = nodes[u].child[side ^ 1];
        if (!aux_root(p))
            nodes[g].child[nodes[g].child[1] == p] = u;
        nodes[u].parent = g;
        nodes[u].child[side ^ 1] = p;
        nodes[p].parent = u;
        nodes[p].child[side] = middle;
        if (middle)
            nodes[middle].parent = p;
        pull(p);
        pull(u);
    }

    void splay(int u) {
        vector<int> chain{u};
        for (int v = u; !aux_root(v);)
            chain.push_back(v = nodes[v].parent);
        for (auto i = chain.rbegin(); i != chain.rend(); ++i)
            push(*i);
        while (!aux_root(u)) {
            int p = nodes[u].parent, g = nodes[p].parent;
            if (!aux_root(p)) {
                bool same = (nodes[g].child[1] == p) == (nodes[p].child[1] == u);
                rotate(same ? p : u);
            }
            rotate(u);
        }
    }

    void access(int u) {
        int last = 0;
        for (int v = u; v; v = nodes[v].parent) {
            splay(v);
            nodes[v].child[1] = last;
            if (last)
                nodes[last].parent = v;
            pull(v);
            last = v;
        }
        splay(u);
    }

    void make_root(int u) {
        access(u);
        flip(u);
    }

    int find_root(int u) {
        access(u);
        push(u);
        while (nodes[u].child[0]) {
            u = nodes[u].child[0];
            push(u);
        }
        splay(u);
        return u;
    }

    bool link(int u, int v) {
        make_root(u);
        if (find_root(v) == u)
            return false;
        nodes[u].parent = v;
        return true;
    }

    bool cut(int u, int v) {
        make_root(u);
        access(v);
        if (nodes[v].child[0] != u || nodes[u].child[1])
            return false;
        nodes[v].child[0] = nodes[u].parent = 0;
        pull(v);
        return true;
    }

    void set(int u, ull value) {
        access(u);
        nodes[u].value = value;
        pull(u);
    }

    optional<ull> path_xor(int u, int v) {
        make_root(u);
        if (find_root(v) != u)
            return nullopt;
        access(v);
        return nodes[v].sum;
    }
};
