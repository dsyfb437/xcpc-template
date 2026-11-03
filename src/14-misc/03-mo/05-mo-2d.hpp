/**
 * @brief 二维莫队（子矩形频次平方和）
 * @complexity 排序 O(hw log(hw) + q log q)，每次移动一条边扫描一行或一列
 * @index 矩形 {top,bottom,left,right}，两维都是左闭右开
 * @idea 前三条边分块，最后一条直接排序。移动矩形边界时，整行或整列加入、移除。
 * @usage auto ans = mo_rectangle_frequency_squares(grid, rectangles);
 *   每个矩形返回 Σ frequency[x]²。
 * @pitfall h、w 是网格长宽。加入一个出现过 c 次的值，答案增加 2c+1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll>
mo_rectangle_frequency_squares(const vector<vector<ll>> &grid,
                               const vector<array<int, 4>> &rectangles) {
    int h = (int)grid.size(), w = h ? (int)grid[0].size() : 0;
    vector<ll> values;
    for (const auto &row : grid)
        values.insert(values.end(), row.begin(), row.end());
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    vector<vector<int>> rank(h, vector<int>(w));
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            rank[i][j] = lower_bound(values.begin(), values.end(), grid[i][j]) -
                         values.begin();

    int block =
        max(1, (int)ceil(max(h, w) / pow(max<size_t>(1, rectangles.size()), 0.25)));
    vector<int> order(rectangles.size());
    iota(order.begin(), order.end(), 0);
    auto key = [&](int id) {
        auto [top, bottom, left, right] = rectangles[id];
        return tuple{top / block, bottom / block, left / block, right};
    };
    sort(order.begin(), order.end(), [&](int i, int j) { return key(i) < key(j); });

    int top = 0, bottom = 0, left = 0, right = 0;
    vector<int> frequency(values.size());
    vector<ll> ans(rectangles.size());
    ll weight = 0;
    auto change = [&](int x, int delta) {
        weight += delta * (2LL * frequency[x] + delta);
        frequency[x] += delta;
    };
    auto row = [&](int i, int delta) {
        for (int j = left; j < right; j++)
            change(rank[i][j], delta);
    };
    auto column = [&](int j, int delta) {
        for (int i = top; i < bottom; i++)
            change(rank[i][j], delta);
    };
    for (int id : order) {
        auto [t, b, l, r] = rectangles[id];
        while (top > t)
            row(--top, 1);
        while (bottom < b)
            row(bottom++, 1);
        while (left > l)
            column(--left, 1);
        while (right < r)
            column(right++, 1);
        while (top < t)
            row(top++, -1);
        while (bottom > b)
            row(--bottom, -1);
        while (left < l)
            column(left++, -1);
        while (right > r)
            column(--right, -1);
        ans[id] = weight;
    }
    return ans;
}
