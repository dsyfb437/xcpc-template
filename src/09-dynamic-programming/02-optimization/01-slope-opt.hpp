/**
 * @brief 斜率优化（单调凸包）
 * @complexity 每次插入和查询均摊 O(1)
 * @index 查所有已插入直线的最小值
 * @idea 斜率从大到小插入，按交点先后删掉永远不会最优的线。查询 x 增大时从队头移走过时直线。
 * @usage MonotoneCHT hull; hull.add(k,b); hull.query(x) 求 min(k*x+b)。
 * @pitfall 斜率单调不增，查询 x 单调不减，查询前至少插入一条线。k、b、x 的绝对值不超过 10^18。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MonotoneCHT {
    struct Line {
        ll slope, intercept;
        __int128 at(ll x) const { return (__int128)slope * x + intercept; }
    };
    deque<Line> lines;

    bool redundant(Line a, Line b, Line c) const {
        __int128 left = ((__int128)b.intercept - a.intercept) * (b.slope - c.slope);
        __int128 right =
            ((__int128)c.intercept - b.intercept) * (a.slope - b.slope);
        return left >= right;
    }

    void add(ll slope, ll intercept) {
        if (!lines.empty() && lines.back().slope == slope) {
            if (lines.back().intercept <= intercept)
                return;
            lines.pop_back();
        }
        Line next{slope, intercept};
        while (lines.size() >= 2 &&
               redundant(lines[lines.size() - 2], lines.back(), next))
            lines.pop_back();
        lines.push_back(next);
    }

    __int128 query(ll x) {
        assert(!lines.empty());
        while (lines.size() >= 2 && lines[1].at(x) <= lines[0].at(x))
            lines.pop_front();
        return lines.front().at(x);
    }
};
