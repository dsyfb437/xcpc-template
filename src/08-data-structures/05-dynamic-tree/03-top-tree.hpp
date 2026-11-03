/**
 * @brief Top Tree（动态森林直径）
 * @complexity 单次均摊 O(log^2 n)
 * @index 原图点 1-based
 * @idea 首选路径上的簇用 Splay 做 compress，虚儿子的簇用有序集合做 rake。每个簇记录路径长度、两端最远距离和直径。
 * @usage DiameterTopTree t(n); t.link(u,v,w); t.cut(u,v); t.set_edge(u,v,w); t.diameter(u)。
 * @pitfall 边权非负，图必须是森林。用独立节点存边权，直径用 __int128。多重集维护虚儿子比直接的 Top Tree 多一层对数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class DiameterTopTree {
    using Wide = __int128;
    struct Cluster {
        Wide length = 0, left = 0, right = 0, diameter = 0;
    };
    struct Node {
        int child[2]{}, parent = 0;
        bool reverse = false;
        ll weight = 0;
        Cluster cluster;
        multiset<Wide> heights, diameters;
    };
    vector<Node> nodes;
    vector<int> recycled;
    map<pair<int, int>, int> edges;

    // compress：连接两条相邻的边界路径。
    static Cluster compress(Cluster a, Cluster b) {
        return {a.length + b.length, max(a.left, a.length + b.left),
                max(b.right, b.length + a.right),
                max({a.diameter, b.diameter, a.right + b.left})};
    }

    void rake(int u, int child, bool add) {
        auto &a = nodes[u];
        auto b = nodes[child].cluster;
        if (add) {
            a.heights.insert(b.left);
            a.diameters.insert(b.diameter);
        } else {
            auto h = a.heights.find(b.left), d = a.diameters.find(b.diameter);
            assert(h != a.heights.end() && d != a.diameters.end());
            a.heights.erase(h);
            a.diameters.erase(d);
        }
    }

    void pull(int u) {
        auto &a = nodes[u];
        Wide first = 0, second = 0, diameter = 0;
        if (!a.heights.empty()) {
            auto it = a.heights.rbegin();
            first = *it;
            if (++it != a.heights.rend())
                second = *it;
            diameter = *a.diameters.rbegin();
        }
        Wide w = a.weight, far = w + first;
        Cluster middle{w, far, far, max(diameter, w + first + second)};
        a.cluster = compress(compress(nodes[a.child[0]].cluster, middle),
                             nodes[a.child[1]].cluster);
    }

    bool aux_root(int u) const {
        int p = nodes[u].parent;
        return !p || (nodes[p].child[0] != u && nodes[p].child[1] != u);
    }

    void flip(int u) {
        if (!u)
            return;
        auto &a = nodes[u];
        swap(a.child[0], a.child[1]);
        swap(a.cluster.left, a.cluster.right);
        a.reverse = !a.reverse;
    }

    void push(int u) {
        auto &a = nodes[u];
        if (a.reverse) {
            flip(a.child[0]);
            flip(a.child[1]);
            a.reverse = false;
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
        vector<int> path{u};
        for (int v = u; !aux_root(v);)
            path.push_back(v = nodes[v].parent);
        for (auto it = path.rbegin(); it != path.rend(); ++it)
            push(*it);
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
            int old = nodes[v].child[1];
            if (old)
                rake(v, old, true);
            if (last)
                rake(v, last, false);
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

    void attach(int u, int v) {
        make_root(u);
        access(v);
        nodes[u].parent = v;
        rake(v, u, true);
        pull(v);
    }

    void detach(int u, int v) {
        make_root(u);
        access(v);
        assert(nodes[v].child[0] == u && !nodes[u].child[1]);
        nodes[v].child[0] = nodes[u].parent = 0;
        pull(v);
    }

  public:
    explicit DiameterTopTree(int n) : nodes(n + 1) {}

    bool link(int u, int v, ll weight) {
        assert(weight >= 0);
        if (find_root(u) == find_root(v))
            return false;
        int edge;
        if (recycled.empty()) {
            edge = (int)nodes.size();
            nodes.emplace_back();
        } else {
            edge = recycled.back();
            recycled.pop_back();
            nodes[edge] = {};
        }
        nodes[edge].weight = weight;
        pull(edge);
        attach(u, edge);
        attach(edge, v);
        edges[minmax(u, v)] = edge;
        return true;
    }

    bool cut(int u, int v) {
        auto it = edges.find(minmax(u, v));
        if (it == edges.end())
            return false;
        int edge = it->second;
        detach(u, edge);
        detach(edge, v);
        recycled.push_back(edge);
        edges.erase(it);
        return true;
    }

    bool set_edge(int u, int v, ll weight) {
        assert(weight >= 0);
        auto it = edges.find(minmax(u, v));
        if (it == edges.end())
            return false;
        int edge = it->second;
        access(edge);
        nodes[edge].weight = weight;
        pull(edge);
        return true;
    }

    Wide diameter(int u) {
        access(u);
        return nodes[u].cluster.diameter;
    }
};
