/**
 * @brief 配对堆（可减键小根堆）
 * @complexity 插入 O(1)，弹出均摊 O(log n)
 * @index push 返回节点编号
 * @idea 合并时让键大的根挂到键小的根下。弹出时先两两合并儿子，再倒序合并结果。减键先断开节点再并回根。
 * @usage PairingHeap heap; int id=heap.push(x); heap.decrease(id,y); heap.top(); heap.pop()。
 * @pitfall top 和 pop 要求非空，decrease 的编号必须仍在堆中且 y 不大于旧值。弹出后的编号不复用。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PairingHeap {
    struct Node {
        ll key = 0;
        int child = 0, previous = 0, next = 0;
    };
    vector<Node> nodes{Node{}};
    int root = 0;

    bool empty() const { return root == 0; }
    ll top() const {
        assert(root);
        return nodes[root].key;
    }

    int meld(int a, int b) {
        if (!a || !b)
            return a ? a : b;
        if (nodes[a].key > nodes[b].key) {
            int t = a;
            a = b;
            b = t;
        }
        int child = nodes[a].child;
        nodes[b].previous = a;
        nodes[b].next = child;
        if (child)
            nodes[child].previous = b;
        nodes[a].child = b;
        return a;
    }

    int push(ll key) {
        int u = (int)nodes.size();
        nodes.push_back({key});
        root = meld(root, u);
        return u;
    }

    void decrease(int u, ll key) {
        assert(key <= nodes[u].key);
        nodes[u].key = key;
        if (u == root)
            return;
        int p = nodes[u].previous, next = nodes[u].next;
        if (nodes[p].child == u)
            nodes[p].child = next;
        else
            nodes[p].next = next;
        if (next)
            nodes[next].previous = p;
        nodes[u].previous = nodes[u].next = 0;
        root = meld(root, u);
    }

    void pop() {
        assert(root);
        vector<int> paired;
        int child = nodes[root].child;
        while (child) {
            int a = child, b = nodes[a].next;
            child = b ? nodes[b].next : 0;
            nodes[a].previous = nodes[a].next = 0;
            if (b)
                nodes[b].previous = nodes[b].next = 0;
            paired.push_back(meld(a, b));
        }
        root = 0;
        for (auto it = paired.rbegin(); it != paired.rend(); ++it)
            root = meld(*it, root);
    }
};
