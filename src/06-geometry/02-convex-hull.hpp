/**
 * @brief 凸包（Andrew）
 * @complexity O(n log n)
 * @index vector 0-based
 * @idea 先排序去重，分别扫出下凸壳和上凸壳。返回逆时针顶点，不重复起点。
 * @usage auto h = convex_hull(p); 函数会排序自己的副本，原数组 p 不变。
 * @pitfall 坐标绝对值不超过 10^12，叉积用 __int128。共线中间点会被删掉；全部共线时只留两端。
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

    Wide turn(const Point &a, const Point &b) const {
        return (a - *this) ^ (b - *this);
    }
};

vector<Point> convex_hull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    int n = p.size();
    if (n <= 2)
        return p;

    vector<Point> h(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && ((h[k - 1] - h[k - 2]) ^ (p[i] - h[k - 2])) <= 0)
            k--;
        h[k++] = p[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && ((h[k - 1] - h[k - 2]) ^ (p[i] - h[k - 2])) <= 0)
            k--;
        h[k++] = p[i];
    }
    h.resize(k - 1);
    return h;
}
