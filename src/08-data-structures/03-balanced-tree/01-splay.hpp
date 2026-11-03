/**
 * @brief Splay 有序多重集
 * @complexity 单次均摊 O(log n)
 * @index 节点编号 0 表示空，第 k 小从 1 起
 * @idea 访问后用单旋或双旋把节点转到根，维护子树大小。删除根时把左子树最大点转上来再接右子树。
 * @usage SplayMultiset t; t.insert(x); t.erase(x); t.count_less(x); t.kth(k)。
 * @pitfall 重复值按次数计，erase 只删一个并返回是否成功。k 超出范围返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SplayMultiset {
    struct Node {
        ll key = 0;
        int child[2]{}, parent = 0, count = 0, size = 0;
    };
    vector<Node> nodes{Node{}};
    int root = 0;

    void pull(int u) {
        auto &x = nodes[u];
        x.size = x.count + nodes[x.child[0]].size + nodes[x.child[1]].size;
    }

    void rotate(int u) {
        int p = nodes[u].parent, g = nodes[p].parent;
        int side = nodes[p].child[1] == u;
        int middle = nodes[u].child[side ^ 1];
        if (g)
            nodes[g].child[nodes[g].child[1] == p] = u;
        else
            root = u;
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
        while (nodes[u].parent) {
            int p = nodes[u].parent, g = nodes[p].parent;
            if (g) {
                bool same = (nodes[g].child[1] == p) == (nodes[p].child[1] == u);
                rotate(same ? p : u);
            }
            rotate(u);
        }
    }

    bool find(ll key) {
        int u = root, last = 0;
        while (u && nodes[u].key != key) {
            last = u;
            u = nodes[u].child[nodes[u].key < key];
        }
        if (u || last)
            splay(u ? u : last);
        return u != 0;
    }

    void insert(ll key) {
        int u = root, parent = 0;
        while (u && nodes[u].key != key) {
            parent = u;
            u = nodes[u].child[nodes[u].key < key];
        }
        if (u) {
            nodes[u].count++;
            pull(u);
        } else {
            u = (int)nodes.size();
            nodes.push_back({key, {}, parent, 1, 1});
            if (parent)
                nodes[parent].child[nodes[parent].key < key] = u;
            else
                root = u;
        }
        splay(u);
    }

    bool erase(ll key) {
        if (!find(key))
            return false;
        if (nodes[root].count > 1) {
            nodes[root].count--;
            pull(root);
            return true;
        }
        int a = nodes[root].child[0], b = nodes[root].child[1];
        if (a)
            nodes[a].parent = 0;
        if (b)
            nodes[b].parent = 0;
        root = a ? a : b;
        if (!a)
            return true;
        while (nodes[a].child[1])
            a = nodes[a].child[1];
        splay(a);
        nodes[root].child[1] = b;
        if (b)
            nodes[b].parent = root;
        pull(root);
        return true;
    }

    int count_less(ll key) {
        int count = 0, u = root, last = 0;
        while (u) {
            last = u;
            auto x = nodes[u];
            if (x.key < key) {
                count += nodes[x.child[0]].size + x.count;
                u = x.child[1];
            } else {
                u = x.child[0];
            }
        }
        if (last)
            splay(last);
        return count;
    }

    optional<ll> kth(int k) {
        if (k <= 0 || k > nodes[root].size)
            return nullopt;
        for (int u = root;;) {
            auto x = nodes[u];
            int left = nodes[x.child[0]].size;
            if (k <= left) {
                u = x.child[0];
            } else if (k <= left + x.count) {
                splay(u);
                return x.key;
            } else {
                k -= left + x.count;
                u = x.child[1];
            }
        }
    }
};
