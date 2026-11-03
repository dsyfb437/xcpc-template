/**
 * @brief 分块（区间加、区间和）
 * @complexity 单次 O(sqrt(n))
 * @index 数组 0-based，区间 [l,r)
 * @idea 整块更新只动懒标记和块和，边角逐项改。查整块时直接取块和。
 * @usage RangeAddBlocks blocks(a); blocks.add(l,r,delta); blocks.sum(l,r)。
 * @pitfall 区间端点合法，允许空区间。元素和加法标记放进 long long，区间和用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct RangeAddBlocks {
    int n, width;
    vector<ll> a, lazy;
    vector<__int128> total;

    explicit RangeAddBlocks(const vector<ll> &values)
        : n((int)values.size()), width(max(1, (int)sqrt(n))), a(values),
          lazy((n + width - 1) / width), total(lazy.size()) {
        for (int i = 0; i < n; i++)
            total[i / width] += a[i];
    }

    void add(int l, int r, ll delta) {
        while (l < r) {
            int block = l / width;
            int end = min(n, (block + 1) * width);
            if (l % width == 0 && end <= r) {
                lazy[block] += delta;
                total[block] += (__int128)(end - l) * delta;
                l = end;
            } else {
                a[l++] += delta;
                total[block] += delta;
            }
        }
    }

    __int128 sum(int l, int r) const {
        __int128 result = 0;
        while (l < r) {
            int block = l / width;
            int end = min(n, (block + 1) * width);
            if (l % width == 0 && end <= r) {
                result += total[block];
                l = end;
            } else {
                result += (__int128)a[l++] + lazy[block];
            }
        }
        return result;
    }
};
