/**
 * @brief AVL 有序集合
 * @complexity 单次 O(log n)
 * @index 节点编号 0 表示空
 * @idea 回溯时检查左右高度差，差为 2 时单旋或双旋。子树大小同时用于顺序统计。
 * @usage AVLTree t; t.insert(x); t.erase(x); t.count_less(x); t.kth(k)。
 * @pitfall 集合去重。删除不存在的键无效，第 k 小从 1 起，越界返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct AVLTree {
    struct Node {
        ll key = 0;
        int child[2]{}, height = 0, size = 0;
    };
    vector<Node> nodes{Node{}};
    int root = 0;

    void pull(int u) {
        auto &x = nodes[u];
        auto l = nodes[x.child[0]], r = nodes[x.child[1]];
        x.height = 1 + max(l.height, r.height);
        x.size = 1 + l.size + r.size;
    }

    int rotate(int u, int dir) {
        int v = nodes[u].child[dir];
        nodes[u].child[dir] = nodes[v].child[dir ^ 1];
        nodes[v].child[dir ^ 1] = u;
        pull(u);
        pull(v);
        return v;
    }

    int balance(int u) {
        pull(u);
        int l = nodes[u].child[0], r = nodes[u].child[1];
        int diff = nodes[l].height - nodes[r].height;
        if (diff >= -1 && diff <= 1)
            return u;
        int dir = diff < 0, v = nodes[u].child[dir];
        if (nodes[nodes[v].child[dir]].height <
            nodes[nodes[v].child[dir ^ 1]].height)
            nodes[u].child[dir] = rotate(v, dir ^ 1);
        return rotate(u, dir);
    }

    int insert_at(int u, ll key) {
        if (!u) {
            nodes.push_back({key, {}, 1, 1});
            return (int)nodes.size() - 1;
        }
        if (key == nodes[u].key)
            return u;
        int dir = nodes[u].key < key;
        nodes[u].child[dir] = insert_at(nodes[u].child[dir], key);
        return balance(u);
    }

    int erase_at(int u, ll key) {
        if (!u)
            return 0;
        if (key == nodes[u].key) {
            int l = nodes[u].child[0], r = nodes[u].child[1];
            if (!l || !r)
                return l ? l : r;
            int next = r;
            while (nodes[next].child[0])
                next = nodes[next].child[0];
            nodes[u].key = nodes[next].key;
            nodes[u].child[1] = erase_at(r, nodes[next].key);
        } else {
            int dir = nodes[u].key < key;
            nodes[u].child[dir] = erase_at(nodes[u].child[dir], key);
        }
        return balance(u);
    }

    void insert(ll key) { root = insert_at(root, key); }
    void erase(ll key) { root = erase_at(root, key); }

    int count_less(ll key) const {
        int answer = 0;
        for (int u = root; u;) {
            auto x = nodes[u];
            if (x.key < key) {
                answer += 1 + nodes[x.child[0]].size;
                u = x.child[1];
            } else {
                u = x.child[0];
            }
        }
        return answer;
    }

    optional<ll> kth(int k) const {
        if (k <= 0 || k > nodes[root].size)
            return nullopt;
        for (int u = root;;) {
            auto x = nodes[u];
            int rank = 1 + nodes[x.child[0]].size;
            if (k == rank)
                return x.key;
            if (k < rank)
                u = x.child[0];
            else {
                k -= rank;
                u = x.child[1];
            }
        }
    }
};
