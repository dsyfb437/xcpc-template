#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"

#include <bits/stdc++.h>
#include "../../alg/06-geometry/convex_hull.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<Point> p(n);
        for (auto &q : p) cin >> q.x >> q.y;
        auto hull = convex_hull(p);
        cout << hull.size() << '\n';
        for (auto &q : hull) cout << q.x << ' ' << q.y << '\n';
    }
}
