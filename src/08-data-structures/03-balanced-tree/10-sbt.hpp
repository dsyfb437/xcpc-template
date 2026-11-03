/**
 * @brief Size Balanced Tree 有序集合
 * @complexity 查询 O(log n)，更新均摊 O(log n)
 * @index 节点编号 0 表示空
 * @idea 检查两侧孙子树是否比另一侧整棵子树还大，失衡时旋转并递归修复受影响的子树。
 * @usage SizeBalancedTree t; t.insert(x); t.erase(x); t.count_less(x); t.kth(k)。
 * @pitfall 集合去重，删除不存在的键无效。第 k 小从 1 起，越界返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SizeBalancedTree {
    struct Node {
        ll key = 0;
        int child[2]{}, size = 0;
    };
    vector<Node> nodes{Node{}};
    int root = 0;

    void pull(int u) {
        auto &a = nodes[u];
        a.size = 1 + nodes[a.child[0]].size + nodes[a.child[1]].size;
    }

    int rotate(int u, int dir) {
        int v = nodes[u].child[dir];
        nodes[u].child[dir] = nodes[v].child[dir ^ 1];
        nodes[v].child[dir ^ 1] = u;
        pull(u);
        pull(v);
        return v;
    }

    int maintain(int u) {
        if (!u)
            return 0;
        for (int dir = 0; dir < 2; dir++) {
            int v = nodes[u].child[dir], other = nodes[u].child[dir ^ 1];
            int limit = nodes[other].size;
            if (nodes[nodes[v].child[dir]].size <= limit) {
                if (nodes[nodes[v].child[dir ^ 1]].size <= limit)
                    continue;
                nodes[u].child[dir] = rotate(v, dir ^ 1);
            }
            u = rotate(u, dir);
            nodes[u].child[0] = maintain(nodes[u].child[0]);
            nodes[u].child[1] = maintain(nodes[u].child[1]);
            pull(u);
            return maintain(u);
        }
        return u;
    }

    int insert_at(int u, ll key) {
        if (!u) {
            nodes.push_back({key, {}, 1});
            return (int)nodes.size() - 1;
        }
        if (nodes[u].key == key)
            return u;
        int dir = nodes[u].key < key;
        nodes[u].child[dir] = insert_at(nodes[u].child[dir], key);
        pull(u);
        return maintain(u);
    }

    int erase_at(int u, ll key) {
        if (!u)
            return 0;
        if (nodes[u].key == key) {
            int a = nodes[u].child[0], b = nodes[u].child[1];
            if (!a || !b)
                return a ? a : b;
            int next = b;
            while (nodes[next].child[0])
                next = nodes[next].child[0];
            nodes[u].key = nodes[next].key;
            nodes[u].child[1] = erase_at(b, nodes[next].key);
        } else {
            int dir = nodes[u].key < key;
            nodes[u].child[dir] = erase_at(nodes[u].child[dir], key);
        }
        pull(u);
        return maintain(u);
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
