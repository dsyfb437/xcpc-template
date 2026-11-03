/**
 * @brief Sqrt Tree（静态区间合并）
 * @complexity 预处理 O(n log log n)，查询 O(1)
 * @index 数组 0-based，区间 [l,r)
 * @idea 每层把区间切成平方根大小的块，存块内前后缀和块间合并结果。用端点异或直接定位首次分属不同块的层。
 * @usage SqrtTree<T,Op> tree(a, identity, op); tree.query(l,r)。
 * @pitfall op 需满足结合律，有单位元，不要求可交换。允许空区间，返回单位元。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T, class Op> struct SqrtTree {
    struct Layer {
        int segment, block, blocks;
        vector<T> prefix, suffix, between;
    };
    int n, length;
    T identity;
    Op op;
    vector<T> values;
    vector<Layer> layers;
    vector<int> layer_at;

    SqrtTree(vector<T> a, T identity, Op op)
        : n((int)a.size()), length((int)bit_ceil((unsigned)n)), identity(identity),
          op(move(op)), values(move(a)), layer_at(bit_width((unsigned)length)) {
        values.resize(length, identity);
        for (int bits = (int)layer_at.size() - 1; bits > 0;) {
            int small = (bits + 1) / 2;
            if (bits == 1)
                small = 0;
            int segment = 1 << bits, block = 1 << small;
            int count = segment / block;
            for (int k = small + 1; k <= bits; k++)
                layer_at[k] = (int)layers.size();
            auto &level = layers.emplace_back(
                Layer{segment, block, count, values, values,
                      vector<T>((length / segment) * count * count, identity)});
            for (int l = 0; l < length; l += block) {
                for (int i = l + 1; i < l + block; i++)
                    level.prefix[i] = this->op(level.prefix[i - 1], values[i]);
                for (int i = l + block - 2; i >= l; i--)
                    level.suffix[i] = this->op(values[i], level.suffix[i + 1]);
            }
            for (int l = 0; l < length; l += segment)
                for (int i = 0; i < count; i++) {
                    T sum = identity;
                    for (int j = i; j < count; j++) {
                        sum = this->op(sum, level.suffix[l + j * block]);
                        int id = ((l / segment) * count + i) * count + j;
                        level.between[id] = sum;
                    }
                }
            bits = small;
        }
    }

    T query(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r)
            return identity;
        if (l == --r)
            return values[l];
        const auto &s = layers[layer_at[bit_width((unsigned)(l ^ r))]];
        int base = l / s.segment;
        int a = l % s.segment / s.block, b = r % s.segment / s.block;
        T result = s.suffix[l];
        if (a + 1 < b) {
            int id = (base * s.blocks + a + 1) * s.blocks + b - 1;
            result = op(result, s.between[id]);
        }
        return op(result, s.prefix[r]);
    }
};
