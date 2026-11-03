/**
 * @brief 后缀平衡树（在线前插）
 * @complexity 前插期望 O(log^2 n)，排名和取后缀期望 O(log n)
 * @index 后缀编号稳定，排名 0-based
 * @idea 新后缀写成首字节加已有后缀。Treap 比较首字节，相同则用子树大小求尾后缀排名。
 * @usage SuffixTreap tree;
 *   int id = tree.prepend(ch);
 *   int rank = tree.rank(id);
 *   int suffix_id = tree.suffix_at(rank);
 * @pitfall 每次只能在整串前端加字节。编号 0 是空后缀，不入树，rank(0)=-1。当前起点是节点数减 id 再减 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct SuffixTreap {
    struct Node {
        unsigned char first = 0;
        ull priority = 0;
        int tail = 0, child[2]{}, parent = 0, size = 0;
    };
    vector<Node> nodes{Node{}};
    mt19937_64 rng;
    int root = 0;

    explicit SuffixTreap(ull seed = random_device{}()) : rng(seed) {}

    int rank(int u) const {
        if (!u)
            return -1;
        int answer = nodes[nodes[u].child[0]].size;
        for (int p = nodes[u].parent; p; u = p, p = nodes[u].parent)
            if (nodes[p].child[1] == u)
                answer += 1 + nodes[nodes[p].child[0]].size;
        return answer;
    }

    void connect(int u, int dir, int v) {
        nodes[u].child[dir] = v;
        if (v)
            nodes[v].parent = u;
    }

    void pull(int u) {
        auto &a = nodes[u];
        a.size = 1 + nodes[a.child[0]].size + nodes[a.child[1]].size;
    }

    int prepend(unsigned char ch) {
        int id = (int)nodes.size(), tail = id - 1;
        int tail_rank = rank(tail);
        nodes.push_back({ch, rng(), tail, {}, 0, 1});
        auto insert = [&](auto &&self, int u) -> int {
            if (!u)
                return id;
            int dir = nodes[u].first != ch ? nodes[u].first < ch
                                           : rank(nodes[u].tail) < tail_rank;
            int v = self(self, nodes[u].child[dir]);
            connect(u, dir, v);
            if (nodes[u].priority < nodes[v].priority) {
                nodes[v].parent = nodes[u].parent;
                connect(u, dir, nodes[v].child[dir ^ 1]);
                connect(v, dir ^ 1, u);
                pull(u);
                u = v;
            }
            pull(u);
            return u;
        };
        root = insert(insert, root);
        nodes[root].parent = 0;
        return id;
    }

    int suffix_at(int k) const {
        assert(0 <= k && k < nodes[root].size);
        for (int u = root;;) {
            int left = nodes[nodes[u].child[0]].size;
            if (left == k)
                return u;
            if (k < left) {
                u = nodes[u].child[0];
            } else {
                k -= left + 1;
                u = nodes[u].child[1];
            }
        }
    }
};
