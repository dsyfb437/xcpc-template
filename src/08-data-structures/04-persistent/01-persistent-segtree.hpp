/**
 * @brief 可持久化线段树（区间和与第 k 小）
 * @complexity 单次 O(log n)
 * @index 值域 [0,n)，根 0 表示全零
 * @idea 点修改只复制根到叶的一条路径。两版本计数相减后，按左子树的计数找第 k 小。
 * @usage PersistentSegTree t(n); int r=t.add(old,pos,delta); t.sum(r,l,rgt); t.kth(newer,older,k)。
 * @pitfall n 为正，sum 的区间半开。kth 要求逐点版本差非负，k 从 1 起且不超过差分总数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PersistentSegTree {
    struct Node {
        int left = 0, right = 0;
        ll sum = 0;
    };
    int n;
    vector<Node> nodes{Node{}};

    explicit PersistentSegTree(int n) : n(n) { assert(n > 0); }

    int add(int root, int position, ll delta) {
        auto dfs = [&](auto &&self, int old, int l, int r) -> int {
            int u = (int)nodes.size();
            nodes.push_back(nodes[old]);
            nodes[u].sum += delta;
            if (r - l == 1)
                return u;
            int mid = (l + r) / 2;
            if (position < mid)
                nodes[u].left = self(self, nodes[old].left, l, mid);
            else
                nodes[u].right = self(self, nodes[old].right, mid, r);
            return u;
        };
        assert(0 <= position && position < n);
        return dfs(dfs, root, 0, n);
    }

    ll sum(int root, int first, int last) const {
        auto dfs = [&](auto &&self, int u, int l, int r) -> ll {
            if (!u || r <= first || last <= l)
                return 0;
            if (first <= l && r <= last)
                return nodes[u].sum;
            int m = (l + r) / 2;
            return self(self, nodes[u].left, l, m) +
                   self(self, nodes[u].right, m, r);
        };
        return dfs(dfs, root, 0, n);
    }

    int kth(int newer, int older, ll k) const {
        assert(k > 0 && k <= nodes[newer].sum - nodes[older].sum);
        int l = 0, r = n;
        while (r - l > 1) {
            int a = nodes[newer].left, b = nodes[older].left;
            ll count = nodes[a].sum - nodes[b].sum;
            int mid = (l + r) / 2;
            if (k <= count) {
                newer = a;
                older = b;
                r = mid;
            } else {
                k -= count;
                newer = nodes[newer].right;
                older = nodes[older].right;
                l = mid;
            }
        }
        return l;
    }
};
