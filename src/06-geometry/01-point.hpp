/**
 * @brief 点与向量
 * @complexity 每个运算 O(1)
 * @idea 整数坐标；点积、叉积、长度平方和转向都用 __int128。
 * @usage Point a{1, 2}, b{3, 4}; auto cr = a ^ b; auto dot = a * b;
 * @pitfall 坐标绝对值不超过 10^12，坐标加减和数乘的结果须放进 long long。接点积、叉积等结果时用 auto，避免转回 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    using Wide = __int128;
    ll x, y;

    Point operator+(const Point &o) const { return {x + o.x, y + o.y}; }
    Point operator-(const Point &o) const { return {x - o.x, y - o.y}; }
    Point operator*(ll k) const { return {x * k, y * k}; }

    Wide operator*(const Point &o) const {
        return Wide(x) * o.x + Wide(y) * o.y;
    } // 点积
    Wide operator^(const Point &o) const {
        return Wide(x) * o.y - Wide(y) * o.x;
    } // 叉积

    bool operator<(const Point &o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator==(const Point &o) const { return x == o.x && y == o.y; }

    Wide norm2() const { return Wide(x) * x + Wide(y) * y; }

    // 以 *this 为原点，a 到 b 的转向：>0 左转，<0 右转，=0 共线。
    Wide turn(const Point &a, const Point &b) const {
        return (a - *this) ^ (b - *this);
    }
};
