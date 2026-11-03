/**
 * @brief 随机增量法（二维线性规划）
 * @complexity 随机期望 O(n)
 * @index 约束 a*x + b*y <= c
 * @idea 打乱约束。若当前最优点被新直线排除，最优点就在这条边界上，把之前的约束投到直线上取可行区间。
 * @usage linear_program_2d(planes, dx, dy, bound, seed) 在给定盒内最大化 dx*x + dy*y，返回 {x,y} 或 nullopt。
 * @pitfall 可行域与 [-bound,bound]^2 取交，bound 为正。输入为长双精度，近似平行时留意 eps。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;

struct LinearConstraint2D {
    ld a, b, c;
};

inline optional<pair<ld, ld>> linear_program_2d(vector<LinearConstraint2D> planes,
                                                ld dx, ld dy, ld bound,
                                                ull seed = random_device{}(),
                                                ld eps = 1e-12L) {
    for (auto &[a, b, c] : planes) {
        ld len = hypot(a, b);
        if (len != 0) {
            a /= len;
            b /= len;
            c /= len;
        } else if (c < 0) {
            return nullopt;
        }
    }
    ld norm = hypot(dx, dy);
    if (norm) {
        dx /= norm;
        dy /= norm;
    }
    mt19937_64 rng(seed);
    shuffle(planes.begin(), planes.end(), rng);
    ld x = dx > 0 ? bound : -bound;
    ld y = dy > 0 ? bound : -bound;
    const LinearConstraint2D box[] = {
        {1, 0, bound}, {-1, 0, bound}, {0, 1, bound}, {0, -1, bound}};
    for (int i = 0; i < (int)planes.size(); i++) {
        auto [a, b, c] = planes[i];
        if (a * x + b * y <= c + eps)
            continue;

        // 边界上的点写成 (a*c, b*c) + t*(-b, a)。
        ld x0 = a * c, y0 = b * c, vx = -b, vy = a;
        ld low = -numeric_limits<ld>::infinity(), high = -low;
        auto clip = [&](LinearConstraint2D h) {
            ld d = h.a * vx + h.b * vy;
            ld r = h.c - h.a * x0 - h.b * y0;
            if (abs(d) <= eps)
                return r >= -eps;
            if (d > 0)
                high = min(high, r / d);
            else
                low = max(low, r / d);
            return low <= high + eps;
        };
        for (auto h : box)
            if (!clip(h))
                return nullopt;
        for (int j = 0; j < i; j++)
            if (!clip(planes[j]))
                return nullopt;

        ld slope = dx * vx + dy * vy;
        // 同值时依次选 x、y 更小的点。
        if (abs(slope) <= eps)
            slope = abs(vx) > eps ? -vx : -vy;
        ld t = slope > 0 ? high : low;
        x = x0 + vx * t;
        y = y0 + vy * t;
    }
    return pair{x, y};
}
