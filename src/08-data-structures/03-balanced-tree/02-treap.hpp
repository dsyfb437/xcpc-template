/**
 * @brief Treap 有序多重集
 * @complexity 单次期望 O(log n)
 * @index 节点编号 0 表示空，第 k 小从 1 起
 * @idea 按键保持二叉搜索树，随机优先级保持堆序。插入后旋转恢复堆序，删除时合并左右子树。
 * @usage TreapMultiset t(seed); t.insert(x); t.erase(x); t.count_less(x); t.kth(k)。
 * @pitfall 重复值按次数计，erase 只删一个。k 超出范围返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct TreapMultiset {
    struct Node {
        ll key = 0;
        ull priority = 0;
        int child[2]{}, count = 0, size = 0;
    };
    vector<Node> nodes{Node{}};
    mt19937_64 rng;
    int root = 0;

    explicit TreapMultiset(ull seed = random_device{}()) : rng(seed) {}

    void pull(int u) {
        auto &x = nodes[u];
        x.size = x.count + nodes[x.child[0]].size + nodes[x.child[1]].size;
    }

    int insert_at(int u, ll key) {
        if (!u) {
            nodes.push_back({key, rng(), {}, 1, 1});
            return (int)nodes.size() - 1;
        }
        if (nodes[u].key == key) {
            nodes[u].count++;
        } else {
            int dir = nodes[u].key < key;
            int v = insert_at(nodes[u].child[dir], key);
            nodes[u].child[dir] = v;
            if (nodes[v].priority > nodes[u].priority) {
                nodes[u].child[dir] = nodes[v].child[dir ^ 1];
                nodes[v].child[dir ^ 1] = u;
                pull(u);
                u = v;
            }
        }
        pull(u);
        return u;
    }

    int merge(int a, int b) {
        if (!a || !b)
            return a ? a : b;
        if (nodes[a].priority >= nodes[b].priority) {
            nodes[a].child[1] = merge(nodes[a].child[1], b);
            pull(a);
            return a;
        }
        nodes[b].child[0] = merge(a, nodes[b].child[0]);
        pull(b);
        return b;
    }

    int erase_at(int u, ll key) {
        if (!u)
            return 0;
        auto &x = nodes[u];
        if (x.key == key) {
            if (--x.count == 0)
                return merge(x.child[0], x.child[1]);
        } else {
            int dir = x.key < key;
            x.child[dir] = erase_at(x.child[dir], key);
        }
        pull(u);
        return u;
    }

    void insert(ll key) { root = insert_at(root, key); }
    void erase(ll key) { root = erase_at(root, key); }

    int count_less(ll key) const {
        int result = 0;
        for (int u = root; u;) {
            auto x = nodes[u];
            if (x.key < key) {
                result += x.count + nodes[x.child[0]].size;
                u = x.child[1];
            } else {
                u = x.child[0];
            }
        }
        return result;
    }

    optional<ll> kth(int k) const {
        if (k <= 0 || k > nodes[root].size)
            return nullopt;
        for (int u = root;;) {
            auto x = nodes[u];
            int left = nodes[x.child[0]].size;
            if (k <= left) {
                u = x.child[0];
            } else if (k <= left + x.count) {
                return x.key;
            } else {
                k -= left + x.count;
                u = x.child[1];
            }
        }
    }
};
