/**
 * @brief 左偏堆（可并小根堆）
 * @complexity 合并、插入和弹出 O(log n)
 * @index 根编号 0 表示空
 * @idea 合并时保留较小的根，只递归合并右儿子。回溯时让左儿子的空距离不小于右儿子。
 * @usage LeftistHeap h; h.push(root,x); h.top(root); h.pop(root); h.meld(root,other)。
 * @pitfall 根变量初值 0，top 和 pop 要求非空。不同根不得共享节点，meld 会清空 other。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LeftistHeap {
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
        auto &u = nodes[a];
        u.right = merge(u.right, b);
        if (nodes[u.left].rank < nodes[u.right].rank)
            swap(u.left, u.right);
        u.rank = nodes[u.right].rank + 1;
        return a;
    }

    void meld(int &a, int &b) {
        a = merge(a, b);
        b = 0;
    }

    void push(int &root, ll key) {
        nodes.push_back({key, 0, 0, 1});
        root = merge(root, (int)nodes.size() - 1);
    }

    ll top(int root) const {
        assert(root);
        return nodes[root].key;
    }

    ll pop(int &root) {
        ll key = top(root);
        root = merge(nodes[root].left, nodes[root].right);
        return key;
    }
};
