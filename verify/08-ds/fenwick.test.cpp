#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <bits/stdc++.h>
#include "../../alg/08-ds/fenwick.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    Fenwick fw(n);
    for (int i = 1; i <= n; i++) {
        ll a;
        cin >> a;
        fw.add(i, a);
    }
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int p;
            ll x;
            cin >> p >> x;
            fw.add(p + 1, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << fw.range(l + 1, r) << '\n';
        }
    }
}
