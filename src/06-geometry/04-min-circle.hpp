/**
 * @brief 最小圆覆盖
 * @complexity 随机期望 O(n)
 * @index 点用实数坐标
 * @idea 打乱点的顺序。遇到不在当前圆内的点，把它放到边界，再按需要固定第二、第三个边界点。
 * @usage minimum_enclosing_circle(points, seed) 返回圆心和半径平方。
 * @pitfall 空点集返回零圆。接近共线和大坐标需按题意调整浮点误差。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;

struct MecPoint {
    ld x, y;
    MecPoint operator+(MecPoint b) const { return {x + b.x, y + b.y}; }
    MecPoint operator-(MecPoint b) const { return {x - b.x, y - b.y}; }
    MecPoint operator*(ld k) const { return {x * k, y * k}; }
    ld norm2() const { return x * x + y * y; }
};

struct EnclosingCircle {
    MecPoint center;
    ld radius2;
};

inline EnclosingCircle minimum_enclosing_circle(vector<MecPoint> p,
                                                ull seed = random_device{}()) {
    mt19937_64 rng(seed);
    shuffle(p.begin(), p.end(), rng);
    constexpr ld eps = 1e-14L;
    auto diameter = [](MecPoint a, MecPoint b) {
        return EnclosingCircle{a + (b - a) * 0.5L, (b - a).norm2() / 4};
    };
    auto through = [&](MecPoint a, MecPoint b, MecPoint c) {
        auto u = b - a, v = c - a;
        ld cross = u.x * v.y - u.y * v.x;
        if (abs(cross) <= eps * sqrt(u.norm2() * v.norm2())) {
            auto first = diameter(a, b), second = diameter(a, c);
            auto third = diameter(b, c);
            if (second.radius2 > first.radius2)
                first = second;
            if (third.radius2 > first.radius2)
                first = third;
            return first;
        }
        ld uu = u.norm2(), vv = v.norm2();
        MecPoint offset{(uu * v.y - vv * u.y) / (2 * cross),
                        (u.x * vv - v.x * uu) / (2 * cross)};
        return EnclosingCircle{a + offset, offset.norm2()};
    };
    auto outside = [&](EnclosingCircle c, MecPoint q) {
        return (q - c.center).norm2() > c.radius2 * (1 + eps);
    };

    EnclosingCircle answer{{0, 0}, 0};
    for (int i = 0; i < (int)p.size(); i++) {
        if (!outside(answer, p[i]))
            continue;
        answer = {p[i], 0};
        for (int j = 0; j < i; j++) {
            if (!outside(answer, p[j]))
                continue;
            answer = diameter(p[i], p[j]);
            for (int k = 0; k < j; k++)
                if (outside(answer, p[k]))
                    answer = through(p[i], p[j], p[k]);
        }
    }
    return answer;
}
