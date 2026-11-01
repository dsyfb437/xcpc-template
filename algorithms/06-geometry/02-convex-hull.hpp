/**
 * @brief 凸包（Andrew）
 * @complexity O(n log n)
 * @index vector 0-based
 *
 * 返回逆时针凸包，不含重复的起点，共线点只保留端点。
 * 点数 <= 2 时原样返回；需要保留边上点就把 <= 0 改成 < 0。
 *
 * 例：
 *   vector<Point> h = convex_hull(p);  // p 会被排序、去重 */
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

    ll turn(const Point &a, const Point &b) const {
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
