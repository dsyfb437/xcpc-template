/**
 * @brief 平面最近点对
 * @complexity O(n log n)
 * @index 点编号按输入从 0 起
 * @idea 按 x 分治。合并时把点按 y 排好，只比较分界线附近且 y 差小于当前最短距离的点。
 * @usage closest_pair(p) 返回 {距离平方, i, j}，不足两点返回 nullopt。
 * @pitfall 整数坐标绝对值不超过 10^12。重合点合法，距离平方用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<tuple<__int128, int, int>>
closest_pair(const vector<pair<ll, ll>> &input) {
    int n = (int)input.size();
    if (n < 2)
        return nullopt;
    struct Item {
        ll x, y;
        int id;
    };
    vector<Item> p;
    for (int i = 0; i < n; i++)
        p.push_back({input[i].first, input[i].second, i});
    sort(p.begin(), p.end(), [](Item a, Item b) {
        return pair{a.x, a.y} < pair{b.x, b.y};
    });
    auto by_y = [](Item a, Item b) { return a.y < b.y; };
    vector<Item> buffer(n);
    __int128 best = (__int128)1 << 120;
    int first = -1, second = -1;
    auto square = [](ll x) { return (__int128)x * x; };
    auto check = [&](Item a, Item b) {
        __int128 d = square(a.x - b.x) + square(a.y - b.y);
        if (d < best) {
            best = d;
            first = a.id;
            second = b.id;
        }
    };
    auto solve = [&](auto &&self, int l, int r) -> void {
        if (r - l <= 3) {
            for (int i = l; i < r; i++)
                for (int j = i + 1; j < r; j++)
                    check(p[i], p[j]);
            sort(p.begin() + l, p.begin() + r, by_y);
            return;
        }
        int m = (l + r) / 2;
        ll middle = p[m].x;
        self(self, l, m);
        self(self, m, r);
        merge(p.begin() + l, p.begin() + m, p.begin() + m, p.begin() + r,
              buffer.begin() + l, by_y);
        copy(buffer.begin() + l, buffer.begin() + r, p.begin() + l);

        vector<Item> strip;
        for (int i = l; i < r; i++) {
            auto u = p[i];
            if (square(u.x - middle) >= best)
                continue;
            for (int j = (int)strip.size() - 1; j >= 0; j--) {
                if (square(u.y - strip[j].y) >= best)
                    break;
                check(u, strip[j]);
            }
            strip.push_back(u);
        }
    };
    solve(solve, 0, n);
    return tuple{best, first, second};
}
