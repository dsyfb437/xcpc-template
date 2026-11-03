/**
 * @brief 重量偏置左偏树（WBLT）
 * @complexity 合并、插入和弹出 O(log n)
 * @index 根编号 0 表示空
 * @idea 让每个节点的右子树大小不超过左子树。合并两个堆时，只沿选中根的右儿子向下。
 * @usage WeightBiasedLeftistTree t; t.push(root,x); t.pop(root); t.meld(root,other)。
 * @pitfall 小根堆，根变量初始为 0。pop 和 top 要求非空，meld 消耗第二个根，两树不得共享节点。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct WeightBiasedLeftistTree {
    struct Node {
        ll key = 0;
        int left = 0, right = 0, size = 0;
    };
    vector<Node> nodes{Node{}};

    int merge(int a, int b) {
        if (!a || !b)
            return a ? a : b;
        if (nodes[a].key > nodes[b].key)
            swap(a, b);
        auto &x = nodes[a];
        x.right = merge(x.right, b);
        if (nodes[x.left].size < nodes[x.right].size)
            swap(x.left, x.right);
        x.size = nodes[x.left].size + nodes[x.right].size + 1;
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
        ll answer = top(root);
        root = merge(nodes[root].left, nodes[root].right);
        return answer;
    }
};
