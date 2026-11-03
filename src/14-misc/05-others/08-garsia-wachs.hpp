/**
 * @brief Garsia–Wachs（相邻合并最小代价）
 * @complexity 期望 O(n log n) 时间，O(n) 空间
 * @index 权重按原顺序传入
 * @idea 找最靠左的 i 使 w[i] ≤ w[i+2]，合并 i 和 i+1。把新权重插回左侧最后一个不小于它的权重之后。
 * @usage long long cost = garsia_wachs_cost(weights);
 * @pitfall 所有权重非负，中间合并值和总代价都要放进 long long。返回代价，序列树里的合并顺序只用于算代价。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace garsia_wachs_detail {
struct Sequence {
    struct Node {
        ll weight = 0, maximum = 0;
        ull priority = 0;
        int left = 0, right = 0, size = 0;
        bool candidate = false, has_candidate = false;
    };

    vector<Node> nodes{1};
    mt19937_64 rng{random_device{}()};

    int make_node(ll w) {
        nodes.push_back({.weight = w, .maximum = w, .priority = rng(), .size = 1});
        return (int)nodes.size() - 1;
    }

    int size(int u) const { return nodes[u].size; }

    void pull(int u) {
        auto &x = nodes[u];
        const auto &l = nodes[x.left], &r = nodes[x.right];
        x.size = l.size + r.size + 1;
        x.maximum = max({l.maximum, x.weight, r.maximum});
        x.has_candidate = l.has_candidate || x.candidate || r.has_candidate;
    }

    int merge(int a, int b) {
        if (a == 0 || b == 0)
            return a + b;
        if (nodes[a].priority > nodes[b].priority) {
            nodes[a].right = merge(nodes[a].right, b);
            pull(a);
            return a;
        }
        nodes[b].left = merge(a, nodes[b].left);
        pull(b);
        return b;
    }

    pair<int, int> split(int u, int count) {
        if (u == 0)
            return {0, 0};
        int left_size = size(nodes[u].left);
        if (count <= left_size) {
            auto [a, b] = split(nodes[u].left, count);
            nodes[u].left = b;
            pull(u);
            return {a, u};
        }
        auto [a, b] = split(nodes[u].right, count - left_size - 1);
        nodes[u].right = a;
        pull(u);
        return {u, b};
    }

    ll weight_at(int u, int i) const {
        int left_size = size(nodes[u].left);
        if (i < left_size)
            return weight_at(nodes[u].left, i);
        if (i == left_size)
            return nodes[u].weight;
        return weight_at(nodes[u].right, i - left_size - 1);
    }

    void set_candidate(int u, int i, bool candidate) {
        int left_size = size(nodes[u].left);
        if (i < left_size)
            set_candidate(nodes[u].left, i, candidate);
        else if (i == left_size)
            nodes[u].candidate = candidate;
        else
            set_candidate(nodes[u].right, i - left_size - 1, candidate);
        pull(u);
    }

    void refresh(int u, int i) {
        if (i < 0 || i >= size(u))
            return;
        int n = size(u);
        bool candidate =
            i + 1 < n && (i + 2 >= n || weight_at(u, i) <= weight_at(u, i + 2));
        set_candidate(u, i, candidate);
    }

    int first_candidate(int u) const {
        const auto &x = nodes[u];
        if (nodes[x.left].has_candidate)
            return first_candidate(x.left);
        int left_size = size(x.left);
        if (x.candidate)
            return left_size;
        return left_size + 1 + first_candidate(x.right);
    }

    int last_at_least(int u, ll weight) const {
        if (u == 0 || nodes[u].maximum < weight)
            return -1;
        const auto &x = nodes[u];
        int left_size = size(x.left);
        if (x.right && nodes[x.right].maximum >= weight)
            return left_size + 1 + last_at_least(x.right, weight);
        if (x.weight >= weight)
            return left_size;
        return last_at_least(x.left, weight);
    }
};
} // namespace garsia_wachs_detail

inline ll garsia_wachs_cost(const vector<ll> &weights) {
    garsia_wachs_detail::Sequence seq;
    int root = 0;
    for (ll w : weights)
        root = seq.merge(root, seq.make_node(w));
    for (int i = 0; i < seq.size(root); i++)
        seq.refresh(root, i);

    ll total = 0;
    while (seq.size(root) > 1) {
        int i = seq.first_candidate(root);
        auto [prefix, rest] = seq.split(root, i);
        auto [pair_nodes, suffix] = seq.split(rest, 2);
        ll sum = seq.weight_at(pair_nodes, 0) + seq.weight_at(pair_nodes, 1);
        int pos = seq.last_at_least(prefix, sum) + 1;
        total += sum;

        // 删、插各只会改变接缝左边两个节点的候选资格。
        root = seq.merge(prefix, suffix);
        seq.refresh(root, i - 2);
        seq.refresh(root, i - 1);
        auto [before, after] = seq.split(root, pos);
        root = seq.merge(seq.merge(before, seq.make_node(sum)), after);
        for (int j = pos - 2; j <= pos; j++)
            seq.refresh(root, j);
    }
    return total;
}
