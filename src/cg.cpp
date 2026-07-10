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
    double len2() const { return x * x + y * y; }
    double len() const { return sqrtl(len2()); }
};
// Andrew 求凸包
{
    int n;
    cin >> n;
    vector<Point> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i].x >> a[i].y;
    sort(a.begin() + 1, a.begin() + 1 + n);

    vector<int> s;
    s.push_back(1);
    for (int i = 2; i <= n; ++i)
    {
        while (s.size() >= 2 && ((a[s.back()] - a[s[s.size() - 2]]) ^ (a[i] - a[s.back()])) <= 0)
            s.pop_back();
        s.push_back(i);
    }
    int tmp = s.size();
    for (int i = n - 1; i >= 1; --i)
    {
        while (s.size() > tmp && ((a[s.back()] - a[s[s.size() - 2]]) ^ (a[i] - a[s.back()])) <= 0)
            s.pop_back();
        s.push_back(i);
    }
}