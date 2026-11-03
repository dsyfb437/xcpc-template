/**
 * @brief 可持久化平衡树（Treap 多重集）
 * @complexity 单次期望 O(log n)
 * @index 根编号 0 表示空，k 从 1 起
 * @idea 分裂、合并和删除时复制访问到的节点，其余子树共享。子树大小用于排名和第 k 小。
 * @usage PersistentTreap t; int v=t.insert(old,x); v=t.erase(v,x); t.count_less(v,x); t.kth(v,k)。
 * @pitfall 重复键逐个计数，erase 只删一个，找不到则保留原版本。所有根都在同一个实例内使用。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct PersistentTreap {
    struct Node {
        ll key = 0;
        ull priority = 0;
        int left = 0, right = 0, size = 0;
    };
    vector<Node> nodes{Node{}};
    mt19937_64 rng;

    explicit PersistentTreap(ull seed = random_device{}()) : rng(seed) {}

    int make(Node node) {
        node.size = 1 + nodes[node.left].size + nodes[node.right].size;
        nodes.push_back(node);
        return (int)nodes.size() - 1;
    }

    pair<int, int> split(int root, ll key) {
        if (!root)
            return {0, 0};
        auto u = nodes[root];
        if (u.key < key) {
            auto [a, b] = split(u.right, key);
            u.right = a;
            return {make(u), b};
        }
        auto [a, b] = split(u.left, key);
        u.left = b;
        return {a, make(u)};
    }

    int merge(int a, int b) {
        if (!a || !b)
            return a ? a : b;
        if (nodes[a].priority > nodes[b].priority) {
            auto u = nodes[a];
            u.right = merge(u.right, b);
            return make(u);
        }
        auto u = nodes[b];
        u.left = merge(a, u.left);
        return make(u);
    }

    int insert(int root, ll key) {
        auto [a, b] = split(root, key);
        int mid = make({key, rng()});
        return merge(merge(a, mid), b);
    }

    int erase(int root, ll key) {
        if (!root)
            return 0;
        auto u = nodes[root];
        if (u.key == key)
            return merge(u.left, u.right);
        if (key < u.key) {
            int left = erase(u.left, key);
            if (left == u.left)
                return root;
            u.left = left;
        } else {
            int right = erase(u.right, key);
            if (right == u.right)
                return root;
            u.right = right;
        }
        return make(u);
    }

    int count_less(int root, ll key) const {
        int answer = 0;
        while (root) {
            auto u = nodes[root];
            if (u.key < key) {
                answer += 1 + nodes[u.left].size;
                root = u.right;
            } else {
                root = u.left;
            }
        }
        return answer;
    }

    ll kth(int root, int k) const {
        assert(k > 0 && k <= nodes[root].size);
        while (true) {
            auto u = nodes[root];
            int rank = nodes[u.left].size + 1;
            if (k == rank)
                return u.key;
            if (k < rank) {
                root = u.left;
            } else {
                root = u.right;
                k -= rank;
            }
        }
    }
};
