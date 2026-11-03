/**
 * @brief 线段树合并 / 分裂
 * @complexity 插入和区间分裂 O(log n)，合并按访问节点数计
 * @index 值域 [0,n)，根编号 0 表示空
 * @idea 动态开点记录各值出现次数。合并递归拼接对应节点，分裂只拆开区间边界。
 * @usage SplitMergeTree t(n); t.add(root,x,d); int other=t.split(root,l,r); t.merge(root,other)。
 * @pitfall 根变量初值为 0，各棵树不共享节点。merge 会清空第二个根，split 从原树移走 [l,r)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SplitMergeTree {
    struct Node {
        int left = 0, right = 0;
        ll sum = 0;
    };
    int n;
    vector<Node> tree{Node{}};

    explicit SplitMergeTree(int n) : n(n) {}

    int make(Node node) {
        tree.push_back(node);
        return (int)tree.size() - 1;
    }

    void pull(int u) {
        tree[u].sum = tree[tree[u].left].sum + tree[tree[u].right].sum;
    }

    void add(int &root, int position, ll delta) {
        auto dfs = [&](auto &&self, int u, int l, int r) -> int {
            if (!u)
                u = make({});
            if (r - l == 1) {
                tree[u].sum += delta;
                return u;
            }
            int m = (l + r) / 2;
            if (position < m)
                tree[u].left = self(self, tree[u].left, l, m);
            else
                tree[u].right = self(self, tree[u].right, m, r);
            pull(u);
            return u;
        };
        root = dfs(dfs, root, 0, n);
    }

    void merge(int &a, int &b) {
        auto dfs = [&](auto &&self, int x, int y, int l, int r) -> int {
            if (!x || !y)
                return x ? x : y;
            if (r - l == 1) {
                tree[x].sum += tree[y].sum;
                return x;
            }
            int m = (l + r) / 2;
            tree[x].left = self(self, tree[x].left, tree[y].left, l, m);
            tree[x].right = self(self, tree[x].right, tree[y].right, m, r);
            pull(x);
            return x;
        };
        a = dfs(dfs, a, b, 0, n);
        b = 0;
    }

    int split(int &root, int first, int last) {
        auto dfs = [&](auto &&self, int u, int l, int r) -> pair<int, int> {
            if (!u || r <= first || last <= l)
                return {u, 0};
            if (first <= l && r <= last)
                return {0, u};
            int m = (l + r) / 2;
            auto [left, a] = self(self, tree[u].left, l, m);
            auto [right, b] = self(self, tree[u].right, m, r);
            int taken = a || b ? make({a, b, tree[a].sum + tree[b].sum}) : 0;
            tree[u] = {left, right, tree[left].sum + tree[right].sum};
            return {left || right ? u : 0, taken};
        };
        auto [keep, take] = dfs(dfs, root, 0, n);
        root = keep;
        return take;
    }

    ll sum(int root, int first, int last) const {
        auto dfs = [&](auto &&self, int u, int l, int r) -> ll {
            if (!u || r <= first || last <= l)
                return 0;
            if (first <= l && r <= last)
                return tree[u].sum;
            int m = (l + r) / 2;
            return self(self, tree[u].left, l, m) + self(self, tree[u].right, m, r);
        };
        return dfs(dfs, root, 0, n);
    }
};
