/**
 * @brief 圆与圆的交点、面积和公切线
 * @complexity O(1)
 * @index 圆由圆心和正半径表示
 * @idea 在圆心连线上求公共弦的位置。面积用扇形减三角形，公切线用单位法向量表示。
 * @usage circle_intersections(a,b)、circle_overlap(a,b)、circle_tangents(a,b)。
 * @pitfall 交点或切线有无限多个时返回 nullopt。无交或无公切线返回空表。浮点 eps 可按题目调。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

struct CirclePoint {
    ld x, y;
    CirclePoint operator+(CirclePoint b) const { return {x + b.x, y + b.y}; }
    CirclePoint operator-(CirclePoint b) const { return {x - b.x, y - b.y}; }
    CirclePoint operator*(ld k) const { return {x * k, y * k}; }
    CirclePoint perpendicular() const { return {-y, x}; }
    ld norm() const { return hypot(x, y); }
};

struct Circle {
    CirclePoint center;
    ld radius;
};

inline optional<vector<CirclePoint>> circle_intersections(Circle a, Circle b,
                                                          ld eps = 1e-12L) {
    auto delta = b.center - a.center;
    ld d = delta.norm(), r = a.radius, s = b.radius;
    if (d <= eps) {
        if (abs(r - s) <= eps)
            return nullopt;
        return vector<CirclePoint>{};
    }
    if (d > r + s + eps || d + eps < abs(r - s))
        return vector<CirclePoint>{};

    auto unit = delta * (1 / d);
    ld t = (d * d + (r - s) * (r + s)) / (2 * d);
    auto middle = a.center + unit * t;
    if (abs(d - (r + s)) <= eps || abs(d - abs(r - s)) <= eps)
        return vector<CirclePoint>{middle};
    auto offset = unit.perpendicular() * sqrt(max(0.L, (r - t) * (r + t)));
    return vector<CirclePoint>{middle + offset, middle - offset};
}

inline ld circle_overlap(Circle a, Circle b) {
    ld d = (a.center - b.center).norm();
    ld r = a.radius, s = b.radius;
    if (d >= r + s)
        return 0;
    if (d <= abs(r - s))
        return acos(-1.L) * min(r, s) * min(r, s);

    ld x = clamp((d * d + (r - s) * (r + s)) / (2 * d * r), -1.L, 1.L);
    ld y = clamp((d * d + (s - r) * (s + r)) / (2 * d * s), -1.L, 1.L);
    return r * r * (acos(x) - x * sqrt(1 - x * x)) +
           s * s * (acos(y) - y * sqrt(1 - y * y));
}

struct CircleTangent {
    CirclePoint point, normal;
};

inline optional<vector<CircleTangent>> circle_tangents(Circle a, Circle b,
                                                       ld eps = 1e-12L) {
    auto delta = b.center - a.center;
    ld d = delta.norm();
    if (d <= eps) {
        if (abs(a.radius - b.radius) <= eps)
            return nullopt;
        return vector<CircleTangent>{};
    }
    auto axis = delta * (1 / d);
    vector<CircleTangent> lines;
    for (int sign = 1; sign >= -1; sign -= 2) {
        ld gap = a.radius - sign * b.radius;
        if (abs(gap) > d + eps)
            continue;
        ld along = clamp(gap / d, -1.L, 1.L);
        ld across = sqrt(1 - along * along);
        int count = abs(abs(gap) - d) <= eps ? 1 : 2;
        if (count == 1)
            across = 0;
        for (int i = 0; i < count; i++) {
            auto normal = axis * along + axis.perpendicular() * across;
            lines.push_back({a.center + normal * a.radius, normal});
            across = -across;
        }
    }
    return lines;
}
