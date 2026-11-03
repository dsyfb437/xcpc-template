/**
 * @brief 矩形面积并（扫描线）
 * @complexity O(n log n)
 * @index 矩形 [x1,x2) × [y1,y2)
 * @idea 按 x 扫描矩形左右边。线段树维护当前被覆盖的 y 长度，乘上下一个事件的横向距离。
 * @usage rectangle_union_area(rects) 返回面积，每项为 {x1,y1,x2,y2}。
 * @pitfall 坐标为整数，绝对值不超过 10^12。跳过零面积矩形，面积用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline __int128 rectangle_union_area(const vector<array<ll, 4>> &rects) {
    struct Event {
        ll x, low, high;
        int delta;
    };
    vector<Event> events;
    vector<ll> ys;
    for (auto [x1, y1, x2, y2] : rects) {
        if (x1 >= x2 || y1 >= y2)
            continue;
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
        ys.push_back(y1);
        ys.push_back(y2);
    }
    if (events.empty())
        return 0;
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(events.begin(), events.end(), [](auto a, auto b) { return a.x < b.x; });

    int n = (int)ys.size() - 1;
    vector<int> cover(4 * n + 4);
    vector<ll> length(4 * n + 4);
    auto update = [&](auto &&self, int node, int l, int r, int a, int b,
                      int delta) -> void {
        if (a <= l && r <= b) {
            cover[node] += delta;
        } else {
            int m = (l + r) / 2;
            if (a < m)
                self(self, node * 2, l, m, a, b, delta);
            if (m < b)
                self(self, node * 2 + 1, m, r, a, b, delta);
        }
        if (cover[node])
            length[node] = ys[r] - ys[l];
        else if (r - l == 1)
            length[node] = 0;
        else
            length[node] = length[node * 2] + length[node * 2 + 1];
    };

    __int128 area = 0;
    ll last = events[0].x;
    for (auto [x, low, high, delta] : events) {
        area += (__int128)(x - last) * length[1];
        int l = lower_bound(ys.begin(), ys.end(), low) - ys.begin();
        int r = lower_bound(ys.begin(), ys.end(), high) - ys.begin();
        update(update, 1, 0, n, l, r, delta);
        last = x;
    }
    return area;
}
