/**
 * @brief 点与向量
 * @complexity -
 *
 * 整数坐标，叉积用 ll；坐标到 1e9 时叉积最大 4e18，接近 ll 上限，
 * 三点共线的极端数据建议把叉积换成 __int128。
 * 下面的凸包默认用这个 Point；半平面交还没写。
 *
 * 例：
 *   Point a{1, 2}, b{3, 4};
 *   ll cr = a ^ b;   // 叉积
 *   ll dt = a * b;   // 点积 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    ll x, y;

    Point operator+(const Point &o) const { return {x + o.x, y + o.y}; }
    Point operator-(const Point &o) const { return {x - o.x, y - o.y}; }
    Point operator*(ll k) const { return {x * k, y * k}; }

    ll operator*(const Point &o) const { return x * o.x + y * o.y; } // 点积
    ll operator^(const Point &o) const { return x * o.y - y * o.x; } // 叉积

    bool operator<(const Point &o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator==(const Point &o) const { return x == o.x && y == o.y; }

    ll norm2() const { return x * x + y * y; }

    // 以 *this 为原点，a 到 b 的转向：>0 左转，<0 右转，=0 共线。
    ll turn(const Point &a, const Point &b) const {
        return (a - *this) ^ (b - *this);
    }
};
