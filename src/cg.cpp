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