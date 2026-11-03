/**
 * @brief 左倾红黑树（LLRB）
 * @complexity 单次 O(log n)
 * @index 节点编号 0 表示空
 * @idea 让红边向左，用翻色拆分临时四节点。删除沿途把红边移到将要进入的一侧，避免黑高减少。
 * @usage RedBlackTree t; t.insert(x); t.erase(x); t.contains(x); t.count_less(x); t.kth(k)。
 * @pitfall 集合去重，删除不存在的键无效。第 k 小从 1 起，越界返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct RedBlackTree {
    struct Node {
        ll key = 0;
        int child[2]{}, size = 0;
        bool red = false;
    };
    vector<Node> nodes{Node{}};
    int root = 0;

    bool red(int u) const { return nodes[u].red; }

    void pull(int u) {
        auto &a = nodes[u];
        a.size = 1 + nodes[a.child[0]].size + nodes[a.child[1]].size;
    }

    int rotate(int u, int dir) {
        int v = nodes[u].child[dir];
        nodes[u].child[dir] = nodes[v].child[dir ^ 1];
        nodes[v].child[dir ^ 1] = u;
        nodes[v].red = nodes[u].red;
        nodes[u].red = true;
        pull(u);
        pull(v);
        return v;
    }

    void flip(int u) {
        nodes[u].red = !nodes[u].red;
        for (int v : nodes[u].child)
            if (v)
                nodes[v].red = !nodes[v].red;
    }

    int balance(int u) {
        if (red(nodes[u].child[1]))
            u = rotate(u, 1);
        int left = nodes[u].child[0];
        if (red(left) && red(nodes[left].child[0]))
            u = rotate(u, 0);
        if (red(nodes[u].child[0]) && red(nodes[u].child[1]))
            flip(u);
        pull(u);
        return u;
    }

    int move_red_left(int u) {
        flip(u);
        int r = nodes[u].child[1];
        if (red(nodes[r].child[0])) {
            nodes[u].child[1] = rotate(r, 0);
            u = rotate(u, 1);
            flip(u);
        }
        return u;
    }

    int move_red_right(int u) {
        flip(u);
        if (red(nodes[nodes[u].child[0]].child[0])) {
            u = rotate(u, 0);
            flip(u);
        }
        return u;
    }

    int insert_at(int u, ll key) {
        if (!u) {
            nodes.push_back({key, {}, 1, true});
            return (int)nodes.size() - 1;
        }
        if (nodes[u].key == key)
            return u;
        int dir = nodes[u].key < key;
        nodes[u].child[dir] = insert_at(nodes[u].child[dir], key);
        return balance(u);
    }

    int erase_min(int u) {
        int l = nodes[u].child[0];
        if (!l)
            return 0;
        if (!red(l) && !red(nodes[l].child[0]))
            u = move_red_left(u);
        nodes[u].child[0] = erase_min(nodes[u].child[0]);
        return balance(u);
    }

    int erase_at(int u, ll key) {
        if (key < nodes[u].key) {
            int l = nodes[u].child[0];
            if (!red(l) && !red(nodes[l].child[0]))
                u = move_red_left(u);
            nodes[u].child[0] = erase_at(nodes[u].child[0], key);
        } else {
            if (red(nodes[u].child[0]))
                u = rotate(u, 0);
            int r = nodes[u].child[1];
            if (key == nodes[u].key && !r)
                return 0;
            if (!red(r) && !red(nodes[r].child[0]))
                u = move_red_right(u);
            if (key == nodes[u].key) {
                int next = nodes[u].child[1];
                while (nodes[next].child[0])
                    next = nodes[next].child[0];
                nodes[u].key = nodes[next].key;
                nodes[u].child[1] = erase_min(nodes[u].child[1]);
            } else {
                nodes[u].child[1] = erase_at(nodes[u].child[1], key);
            }
        }
        return balance(u);
    }

    bool contains(ll key) const {
        int u = root;
        while (u && nodes[u].key != key)
            u = nodes[u].child[nodes[u].key < key];
        return u != 0;
    }

    void insert(ll key) {
        root = insert_at(root, key);
        nodes[root].red = false;
    }

    void erase(ll key) {
        if (!contains(key))
            return;
        if (!red(nodes[root].child[0]) && !red(nodes[root].child[1]))
            nodes[root].red = true;
        root = erase_at(root, key);
        nodes[root].red = false;
    }

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
