#include <bits/stdc++.h>
using namespace std;

const double pi = 3.1415926535897932384;
using ll = long long;
struct Point
{
    ll x, y;
    bool operator<(const Point &a) const
    {
        if (x == a.x)
            return y < a.y;
        return x < a.x;
    }
    Point operator-(const Point &a) const { return {x - a.x, y - a.y}; }
    ll operator^(const Point &a) const { return x * a.y - y * a.x; }
    ll toleft(const Point &a) const
    {
        const auto t = (*this) ^ a;
        return (t > 0) - (t < 0);
    }
    double len2() const { return x * x + y * y; }
    double len() const { return sqrtl(len2()); }
};

// Andrew 求凸包
//O(n log n)
vector<Point> convexHull(vector<Point> p) {
    sort(p.begin(), p.end());
    vector<Point> h;
    for (auto& pt : p) {
        while (h.size() >= 2 && 
               ((h.back() - h[h.size()-2]) ^ (pt - h.back())) <= 0)
            h.pop_back();
        h.push_back(pt);
    }
    int lower = h.size();
    for (int i = (int)p.size() - 2; i >= 0; --i) {
        while (h.size() > lower && 
               ((h.back() - h[h.size()-2]) ^ (p[i] - h.back())) <= 0)
            h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back();
    return h;
}

// 最大三角形（旋转卡壳） 
//O(n^2)
vector<Point> maxTriangle(const vector<Point>& h) {
    int n = h.size();
    ll maxx = -1;
    Point p0, p1, p2;
    for (int i = 0; i < n; ++i) {
        int k = (i + 2) % n;
        for (int j = (i + 1) % n; j != i; j = (j + 1) % n) {
            if (j == k) k = (k + 1) % n;
            while (1) {
                int nk = (k + 1) % n;
                if (nk == i) break;
                ll cur = abs((h[k] - h[i]) ^ (h[k] - h[j]));
                ll nxt = abs((h[nk] - h[i]) ^ (h[nk] - h[j]));
                if (nxt > cur)
                    k = nk;
                else
                    break;
            }
            ll A = abs((h[k] - h[i]) ^ (h[k] - h[j]));
            if (A > maxx) {
                maxx = A;
                p0 = h[i];
                p1 = h[j];
                p2 = h[k];
            }
        }
    }
    return {p0, p1, p2};
}