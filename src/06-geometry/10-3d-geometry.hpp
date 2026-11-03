/**
 * @brief 三维点、向量与平面
 * @complexity 每个运算 O(1)
 * @index 平面用一点与法向量表示
 * @idea 点积投影，叉积取垂直向量，混合积求有向体积。直线交平面只需求一个参数。
 * @usage Point3 支持 dot、cross、norm。line_plane_intersection(p,v,q,n) 求直线和平面的唯一交点。
 * @pitfall 用 long double。直线方向 v 和平面法向量 n 非零，平行或重合时返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

struct Point3 {
    ld x, y, z;

    Point3 operator+(Point3 b) const { return {x + b.x, y + b.y, z + b.z}; }
    Point3 operator-(Point3 b) const { return {x - b.x, y - b.y, z - b.z}; }
    Point3 operator*(ld k) const { return {x * k, y * k, z * k}; }

    ld dot(Point3 b) const { return x * b.x + y * b.y + z * b.z; }
    Point3 cross(Point3 b) const {
        return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
    }
    ld norm() const { return hypot(x, y, z); }
};

inline ld signed_tetrahedron_volume(Point3 a, Point3 b, Point3 c, Point3 d) {
    return (b - a).cross(c - a).dot(d - a) / 6;
}

inline ld point_plane_distance(Point3 p, Point3 q, Point3 normal) {
    return abs((p - q).dot(normal)) / normal.norm();
}

inline optional<Point3> line_plane_intersection(Point3 p, Point3 direction,
                                                Point3 q, Point3 normal,
                                                ld eps = 1e-12L) {
    ld denominator = direction.dot(normal);
    if (abs(denominator) <= eps * direction.norm() * normal.norm())
        return nullopt;
    return p + direction * ((q - p).dot(normal) / denominator);
}
