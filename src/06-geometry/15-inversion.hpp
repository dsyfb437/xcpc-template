/**
 * @brief 圆反演（点、直线与圆）
 * @complexity O(1)
 * @index 直线用 normal·(x-origin)=offset 表示
 * @idea 点沿原射线移动，原距离与新距离的乘积为 R^2。过反演中心的圆变直线，不过中心的圆仍为圆。
 * @usage invert_point、invert_line、invert_circle 都传 origin 和 R。后两者返回直线或圆的 variant。
 * @pitfall R 和圆半径为正，法向量非零。中心点无有限反演像。eps 是判断经过中心的距离容差。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

struct InversionPoint {
    ld x, y;
    InversionPoint operator+(InversionPoint p) const { return {x + p.x, y + p.y}; }
    InversionPoint operator-(InversionPoint p) const { return {x - p.x, y - p.y}; }
    InversionPoint operator*(ld k) const { return {x * k, y * k}; }
    ld norm() const { return hypot(x, y); }
};

struct InversionLine {
    InversionPoint normal;
    ld offset;
};

struct InversionCircle {
    InversionPoint center;
    ld radius;
};

using InversionCurve = variant<InversionLine, InversionCircle>;

inline optional<InversionPoint> invert_point(InversionPoint p,
                                             InversionPoint origin, ld R) {
    auto v = p - origin;
    ld d = v.norm();
    if (d == 0)
        return nullopt;
    return origin + v * ((R / d) * (R / d));
}

inline InversionCurve invert_line(InversionLine line, InversionPoint origin, ld R,
                                  ld eps = 1e-12L) {
    ld len = line.normal.norm();
    if (abs(line.offset) <= eps * len)
        return line;
    ld scale = R * R / (2 * line.offset);
    return InversionCircle{origin + line.normal * scale, abs(scale) * len};
}

inline InversionCurve invert_circle(InversionCircle circle, InversionPoint origin,
                                    ld R, ld eps = 1e-12L) {
    auto v = circle.center - origin;
    ld d = v.norm(), r = circle.radius;
    if (d != 0 && abs(d - r) <= eps)
        return InversionLine{v, R * R / 2};
    ld scale = R * R / ((d - r) * (d + r));
    return InversionCircle{origin + v * scale, abs(scale) * r};
}
