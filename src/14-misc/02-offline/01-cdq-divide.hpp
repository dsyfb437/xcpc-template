/**
 * @brief CDQ 分治（三维偏序）
 * @complexity O(n log² n) 时间，O(n) 空间
 * @index 按输入顺序返回
 * @idea 先按 (x,y,z) 排序并合并相同点。分治时把左半的贡献扫给右半，按 y 扫描，用树状数组统计 z。
 * @usage auto count = dominance_3d(points);
 *   count[i] 是三维都 ≤ points[i] 的其他点数。
 * @pitfall 重合的其他点也要算。每层做完要撤销这一层加进树状数组的点。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> dominance_3d(const vector<array<ll, 3>> &points) {
    int n = (int)points.size();
    vector<ll> zs;
    vector<int> order(n), group(n);
    for (int i = 0; i < n; i++) {
        zs.push_back(points[i][2]);
        order[i] = i;
    }
    sort(zs.begin(), zs.end());
    zs.erase(unique(zs.begin(), zs.end()), zs.end());
    sort(order.begin(), order.end(),
         [&](int a, int b) { return points[a] < points[b]; });

    struct Point {
        ll y;
        int z, id, copies;
    };
    vector<Point> a;
    for (int i = 0; i < n; i++) {
        int id = order[i];
        if (i == 0 || points[id] != points[order[i - 1]]) {
            auto [x, y, z] = points[id];
            int rank = lower_bound(zs.begin(), zs.end(), z) - zs.begin() + 1;
            a.push_back({y, rank, (int)a.size(), 0});
        }
        group[id] = a.back().id;
        a.back().copies++;
    }

    vector<int> count(a.size()), bit(zs.size() + 1);
    vector<Point> scratch(a.size());
    for (const auto &p : a)
        count[p.id] = p.copies - 1;
    auto add = [&](int pos, int delta) {
        for (; pos < (int)bit.size(); pos += pos & -pos)
            bit[pos] += delta;
    };
    auto sum = [&](int pos) {
        int total = 0;
        for (; pos > 0; pos -= pos & -pos)
            total += bit[pos];
        return total;
    };
    auto by_y = [](const Point &p, const Point &q) { return p.y < q.y; };

    auto cdq = [&](auto &&self, int l, int r) -> void {
        if (r - l <= 1)
            return;
        int mid = (l + r) / 2;
        self(self, l, mid);
        self(self, mid, r);
        int i = l;
        for (int j = mid; j < r; j++) {
            while (i < mid && a[i].y <= a[j].y) {
                add(a[i].z, a[i].copies);
                i++;
            }
            count[a[j].id] += sum(a[j].z);
        }
        for (int j = l; j < i; j++)
            add(a[j].z, -a[j].copies);
        merge(a.begin() + l, a.begin() + mid, a.begin() + mid, a.begin() + r,
              scratch.begin() + l, by_y);
        copy(scratch.begin() + l, scratch.begin() + r, a.begin() + l);
    };
    cdq(cdq, 0, (int)a.size());

    vector<int> answer(n);
    for (int i = 0; i < n; i++)
        answer[i] = count[group[i]];
    return answer;
}
