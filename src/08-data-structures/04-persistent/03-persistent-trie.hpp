/**
 * @brief 可持久化二进制 Trie
 * @complexity 插入和查询 O(32)
 * @index 无符号 32 位键，根 0 表示空
 * @idea 按二进制位复制插入路径。用两版本的子树计数差限制候选集合，尽量走与 x 相反的位。
 * @usage PersistentXorTrie t; int r=t.insert(old,x); t.max_xor(newer,older,x) 查最大异或值。
 * @pitfall older 的多重集必须包含于 newer。版本差为空时返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PersistentXorTrie {
    struct Node {
        int child[2]{};
        int count = 0;
    };
    vector<Node> nodes{Node{}};

    int clone(int root) {
        nodes.push_back(nodes[root]);
        nodes.back().count++;
        return (int)nodes.size() - 1;
    }

    int insert(int old, unsigned value) {
        int root = clone(old), u = root;
        for (int bit = 31; bit >= 0; bit--) {
            int b = value >> bit & 1U;
            old = nodes[old].child[b];
            int v = clone(old);
            nodes[u].child[b] = v;
            u = v;
        }
        return root;
    }

    optional<unsigned> max_xor(int newer, int older, unsigned value) const {
        if (nodes[newer].count == nodes[older].count)
            return nullopt;
        unsigned result = 0;
        for (int bit = 31; bit >= 0; bit--) {
            int b = (value >> bit & 1U) ^ 1U;
            int a = nodes[newer].child[b], c = nodes[older].child[b];
            if (nodes[a].count > nodes[c].count)
                result |= unsigned{1} << bit;
            else
                b ^= 1;
            newer = nodes[newer].child[b];
            older = nodes[older].child[b];
        }
        return result;
    }
};
