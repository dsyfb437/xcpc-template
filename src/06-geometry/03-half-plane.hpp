/**
 * @brief 半平面交
 * @complexity O(n log n)
 * @index 每条有向直线的左侧可行
 * @idea 按方向角排序，用双端队列维护有效边界。新边排除队头或队尾交点时，弹掉不再有效的边。
 * @usage half_plane_intersection(planes, bound) 返回逆时针交多边形，与 [-bound,bound]^2 相交。
 * @pitfall direction 非零，bound 为正。空交或退化成交点、线段时返回空表。浮点 eps 需按坐标量级调整。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

struct HpiPoint {
    ld x, y;
    HpiPoint operator+(HpiPoint b) const { return {x + b.x, y + b.y}; }
    HpiPoint operator-(HpiPoint b) const { return {x - b.x, y - b.y}; }
    HpiPoint operator*(ld k) const { return {x * k, y * k}; }
};

struct HalfPlane {
    HpiPoint point, direction;
};

inline vector<HpiPoint> half_plane_intersection(vector<HalfPlane> planes, ld bound,
                                                ld eps = 1e-12L) {
    planes.push_back({{-bound, -bound}, {1, 0}});
    planes.push_back({{bound, -bound}, {0, 1}});
    planes.push_back({{bound, bound}, {-1, 0}});
    planes.push_back({{-bound, bound}, {0, -1}});
    auto cross = [](HpiPoint a, HpiPoint b) { return a.x * b.y - a.y * b.x; };
    auto dot = [](HpiPoint a, HpiPoint b) { return a.x * b.x + a.y * b.y; };
    auto outside = [&](HalfPlane a, HpiPoint p) {
        return cross(a.direction, p - a.point) < -eps;
    };
    auto intersect = [&](HalfPlane a, HalfPlane b) {
        ld t =
            cross(b.point - a.point, b.direction) / cross(a.direction, b.direction);
        return a.point + a.direction * t;
    };
    for (auto &h : planes) {
        // 先缩放再单位化，避免极大方向的长度或极小长度的倒数溢出。
        ld scale = max(abs(h.direction.x), abs(h.direction.y));
        h.direction = {h.direction.x / scale, h.direction.y / scale};
        ld len = hypot(h.direction.x, h.direction.y);
        h.direction = {h.direction.x / len, h.direction.y / len};
    }
    auto angle = [](HalfPlane h) {
        ld a = atan2(h.direction.y, h.direction.x);
        return a < 0 ? a + 2 * acos(-1.L) : a;
    };
    sort(planes.begin(), planes.end(),
         [&](HalfPlane a, HalfPlane b) { return angle(a) < angle(b); });

    // 同向边恰好跨过极角的起点时，先保留更紧的约束。
    while (planes.size() >= 2 &&
           abs(cross(planes.front().direction, planes.back().direction)) <= eps &&
           dot(planes.front().direction, planes.back().direction) > 0) {
        if (outside(planes.back(), planes.front().point))
            planes.front().point = planes.back().point;
        planes.pop_back();
    }

    deque<HalfPlane> active;
    for (auto h : planes) {
        while (active.size() >= 2 &&
               outside(h, intersect(active[active.size() - 2], active.back())))
            active.pop_back();
        while (active.size() >= 2 && outside(h, intersect(active[0], active[1])))
            active.pop_front();

        if (!active.empty() &&
            abs(cross(active.back().direction, h.direction)) <= eps) {
            if (dot(active.back().direction, h.direction) < 0)
                return {};
            if (!outside(h, active.back().point))
                continue;
            active.pop_back();
        }
        active.push_back(h);
    }
    while (active.size() >= 3 &&
           outside(active.front(),
                   intersect(active[active.size() - 2], active.back())))
        active.pop_back();
    while (active.size() >= 3 &&
           outside(active.back(), intersect(active[0], active[1])))
        active.pop_front();
    if (active.size() < 3)
        return {};

    vector<HpiPoint> polygon;
    for (int i = 0; i < (int)active.size(); i++) {
        auto a = active[i], b = active[(i + 1) % active.size()];
        if (abs(cross(a.direction, b.direction)) <= eps)
            return {};
        auto p = intersect(a, b);
        if (polygon.empty() ||
            hypot(p.x - polygon.back().x, p.y - polygon.back().y) > eps)
            polygon.push_back(p);
    }
    if (polygon.size() > 1 && hypot(polygon.front().x - polygon.back().x,
                                    polygon.front().y - polygon.back().y) <= eps)
        polygon.pop_back();
    ld area2 = 0;
    for (int i = 1; i + 1 < (int)polygon.size(); i++)
        area2 += cross(polygon[i] - polygon[0], polygon[i + 1] - polygon[0]);
    if (area2 <= eps * eps)
        return {};
    return polygon;
}
