/**
 * @brief 替罪羊树（有序多重集）
 * @complexity 单次均摊 O(log n)
 * @index 编号 0 表示空，k 从 1 起
 * @idea 用计数做惰性删除。子树太偏或失效点太多时，中序收集活点并重新建成平衡树。
 * @usage ScapegoatMultiset t; t.insert(x); t.erase(x); t.count_less(x); t.kth(k)。
 * @pitfall 重复值按次数计，erase 只删一个。k 超出范围返回 nullopt，删除的节点编号不回收。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct ScapegoatMultiset {
    struct Node {
        ll key = 0;
        int child[2]{}, count = 0, size = 0, total = 0, alive = 0;
    };
    vector<Node> nodes{Node{}};
    int root = 0;

    void pull(int u) {
        auto &a = nodes[u];
        const auto &l = nodes[a.child[0]], &r = nodes[a.child[1]];
        a.size = l.size + r.size + a.count;
        a.total = l.total + r.total + 1;
        a.alive = l.alive + r.alive + (a.count > 0);
    }

    int balance(int u) {
        pull(u);
        const auto &a = nodes[u];
        if (4LL * nodes[a.child[0]].total <= 3LL * a.total &&
            4LL * nodes[a.child[1]].total <= 3LL * a.total &&
            4LL * a.alive >= 3LL * a.total)
            return u;
        vector<int> order;
        auto collect = [&](auto &&self, int x) -> void {
            if (!x)
                return;
            self(self, nodes[x].child[0]);
            if (nodes[x].count)
                order.push_back(x);
            self(self, nodes[x].child[1]);
        };
        collect(collect, u);
        auto build = [&](auto &&self, int l, int r) -> int {
            if (l == r)
                return 0;
            int m = (l + r) / 2, x = order[m];
            nodes[x].child[0] = self(self, l, m);
            nodes[x].child[1] = self(self, m + 1, r);
            pull(x);
            return x;
        };
        return build(build, 0, (int)order.size());
    }

    int modify(int u, ll key, int delta) {
        if (!u) {
            if (delta < 0)
                return 0;
            nodes.push_back({key, {}, 1, 1, 1, 1});
            return (int)nodes.size() - 1;
        }
        if (nodes[u].key == key) {
            if (delta > 0 || nodes[u].count > 0)
                nodes[u].count += delta;
        } else {
            int dir = nodes[u].key < key;
            nodes[u].child[dir] = modify(nodes[u].child[dir], key, delta);
        }
        return balance(u);
    }

    void insert(ll x) { root = modify(root, x, 1); }
    void erase(ll x) { root = modify(root, x, -1); }

    int count_less(ll x) const {
        int answer = 0;
        for (int u = root; u;) {
            auto a = nodes[u];
            if (a.key < x) {
                answer += nodes[a.child[0]].size + a.count;
                u = a.child[1];
            } else {
                u = a.child[0];
            }
        }
        return answer;
    }

    optional<ll> kth(int k) const {
        if (k <= 0 || k > nodes[root].size)
            return nullopt;
        for (int u = root;;) {
            auto a = nodes[u];
            int left = nodes[a.child[0]].size;
            if (k <= left) {
                u = a.child[0];
            } else if (k <= left + a.count) {
                return a.key;
            } else {
                k -= left + a.count;
                u = a.child[1];
            }
        }
    }
};
