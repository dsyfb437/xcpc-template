/**
 * @brief 析合树（排列的强区间树）
 * @complexity O(n^2) 时间，O(n) 额外空间
 * @index 位置区间 [l,r)，点编号 0-based
 * @idea 连续段的最大值减最小值等于长度减一时是公共区间。筛掉与其他公共区间真交叉的段，剩余段按包含关系组成树。
 * @usage divide_combine_tree(p) 返回节点表，根编号为 0。kind 为 leaf、increasing、decreasing 或 prime。
 * @pitfall p 是 0 到 n-1 或 1 到 n 的排列。此版为平方时间，适合中小规模。空输入返回空表。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DivideCombineNode {
    enum Kind { leaf, increasing, decreasing, prime };
    int l, r, low, high;
    Kind kind;
    vector<int> children;
};

inline vector<DivideCombineNode> divide_combine_tree(const vector<int> &p) {
    using N = DivideCombineNode;
    int n = (int)p.size();
    vector<int> first(n, -1), last(n);
    for (int l = 0; l < n; l++) {
        int low = p[l], high = p[l];
        for (int r = l; r < n; r++) {
            low = min(low, p[r]);
            high = max(high, p[r]);
            if (high - low == r - l) {
                if (first[r] == -1)
                    first[r] = l;
                last[l] = r;
            }
        }
    }
    vector<N> tree;
    vector<int> stack;
    for (int l = 0; l < n; l++) {
        int low = p[l], high = p[l], left = l, right = l;
        vector<N> row;
        for (int r = l; r < n; r++) {
            low = min(low, p[r]);
            high = max(high, p[r]);
            if (r != l) {
                left = min(left, first[r - 1]);
                right = max(right, last[r]);
            }
            if (high - low == r - l && left >= l && right <= r)
                row.push_back(
                    {l, r + 1, low, high, l == r ? N::leaf : N::prime, {}});
        }
        for (auto it = row.rbegin(); it != row.rend(); ++it) {
            int id = (int)tree.size();
            while (!stack.empty() && tree[stack.back()].r <= l)
                stack.pop_back();
            if (!stack.empty())
                tree[stack.back()].children.push_back(id);
            tree.push_back(*it);
            stack.push_back(id);
        }
    }
    for (auto &node : tree) {
        if (node.kind == N::leaf)
            continue;
        bool increasing = true, decreasing = true;
        for (int i = 1; i < (int)node.children.size(); i++) {
            auto &a = tree[node.children[i - 1]], &b = tree[node.children[i]];
            increasing &= a.high < b.low;
            decreasing &= a.low > b.high;
        }
        if (increasing)
            node.kind = N::increasing;
        else if (decreasing)
            node.kind = N::decreasing;
    }
    return tree;
}
