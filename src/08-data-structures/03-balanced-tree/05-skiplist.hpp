/**
 * @brief 跳表（有序多重集）
 * @complexity 单次期望 O(log n)
 * @index 节点编号 0 用作表头和空指针
 * @idea 从最高层向右找，走不动就下降。新节点高度随机，修改时只接回有这层的前驱。
 * @usage SkipList s(seed); s.insert(x); s.erase(x); s.count(x); s.lower_bound(x)。
 * @pitfall 允许重复，erase 只删一个。lower_bound 找不到时返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct SkipList {
    static constexpr int levels = 32;
    struct Node {
        ll key;
        int count;
        vector<int> next;
    };
    vector<Node> nodes{{0, 0, vector<int>(levels)}};
    mt19937_64 rng;

    explicit SkipList(ull seed = random_device{}()) : rng(seed) {}

    array<int, levels> predecessors(ll key) const {
        array<int, levels> result{};
        int u = 0;
        for (int k = levels - 1; k >= 0; k--) {
            while (nodes[u].next[k] && nodes[nodes[u].next[k]].key < key)
                u = nodes[u].next[k];
            result[k] = u;
        }
        return result;
    }

    void insert(ll key) {
        auto before = predecessors(key);
        int u = nodes[before[0]].next[0];
        if (u && nodes[u].key == key) {
            nodes[u].count++;
            return;
        }
        int height = min(levels, 1 + countr_one(rng()));
        u = (int)nodes.size();
        nodes.push_back({key, 1, vector<int>(height)});
        for (int k = 0; k < height; k++) {
            nodes[u].next[k] = nodes[before[k]].next[k];
            nodes[before[k]].next[k] = u;
        }
    }

    bool erase(ll key) {
        auto before = predecessors(key);
        int u = nodes[before[0]].next[0];
        if (!u || nodes[u].key != key)
            return false;
        if (--nodes[u].count == 0)
            for (int k = 0; k < (int)nodes[u].next.size(); k++)
                nodes[before[k]].next[k] = nodes[u].next[k];
        return true;
    }

    int count(ll key) const {
        int u = nodes[predecessors(key)[0]].next[0];
        return u && nodes[u].key == key ? nodes[u].count : 0;
    }

    optional<ll> lower_bound(ll key) const {
        int u = nodes[predecessors(key)[0]].next[0];
        if (!u)
            return nullopt;
        return nodes[u].key;
    }
};
