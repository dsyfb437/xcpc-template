/**
 * @brief 猫树（静态区间合并）
 * @complexity 预处理 O(n log n)，查询 O(1)
 * @index 数组 0-based，非空区间 [l,r)
 * @idea 每层以中点为界，左边存后缀，右边存前缀。端点异或的最高位直接给出分界层。
 * @usage CatTree<T,Op> t(a,op); t.query(l,r)。
 * @pitfall op 只需满足结合律，不要求可交换，也不需要单位元。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T, class Op> struct CatTree {
    vector<T> values;
    vector<vector<T>> table;
    Op op;

    CatTree(vector<T> a, Op op) : values(move(a)), op(move(op)) {
        int n = (int)values.size();
        for (int half = 1; half < n; half *= 2) {
            auto &row = table.emplace_back(values);
            for (int l = 0; l < n; l += 2 * half) {
                int mid = min(l + half, n), r = min(l + 2 * half, n);
                for (int i = mid - 2; i >= l; i--)
                    row[i] = this->op(values[i], row[i + 1]);
                for (int i = mid + 1; i < r; i++)
                    row[i] = this->op(row[i - 1], values[i]);
            }
        }
    }

    T query(int l, int r) const {
        assert(0 <= l && l < r && r <= (int)values.size());
        if (l == --r)
            return values[l];
        int level = bit_width((unsigned)(l ^ r)) - 1;
        return op(table[level][l], table[level][r]);
    }
};
