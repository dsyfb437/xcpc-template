/**
 * @brief 可持久化可并堆（左偏堆）
 * @complexity 合并、插入、弹出 O(log n)
 * @index 根 0 表示空
 * @idea 每次选键更小的根，只复制右链上访问到的节点，回溯时恢复左偏性质。
 * @usage PersistentHeap h; int r=h.push(old,x); h.top(r); r=h.pop(r); r=h.merge(a,b)。
 * @pitfall 小根堆，top 和 pop 要求非空。两版本允许共享节点，合并按两份多重集计数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PersistentHeap {
    struct Node {
        ll key = 0;
        int left = 0, right = 0, rank = 0;
    };
    vector<Node> nodes{Node{}};

    int merge(int a, int b) {
        if (!a || !b)
            return a ? a : b;
        if (nodes[a].key > nodes[b].key)
            swap(a, b);
        Node u = nodes[a];
        u.right = merge(u.right, b);
        if (nodes[u.left].rank < nodes[u.right].rank)
            swap(u.left, u.right);
        u.rank = nodes[u.right].rank + 1;
        nodes.push_back(u);
        return (int)nodes.size() - 1;
    }

    int push(int root, ll key) {
        nodes.push_back({key, 0, 0, 1});
        return merge(root, (int)nodes.size() - 1);
    }

    ll top(int root) const {
        assert(root);
        return nodes[root].key;
    }

    int pop(int root) {
        assert(root);
        return merge(nodes[root].left, nodes[root].right);
    }
};
