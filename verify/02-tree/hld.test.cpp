#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"

#include <bits/stdc++.h>
#include "../../alg/02-tree/hld.hpp"
#include "../../alg/08-ds/fenwick.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        u++, v++;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    HLD hld(g, 1);
    Fenwick bit(n);
    for (int i = 1; i <= n; i++)
        bit.add(hld.pos[i] + 1, a[i]);
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int u;
            ll x;
            cin >> u >> x;
            u++;
            bit.add(hld.pos[u] + 1, x);
        } else {
            int u, v;
            cin >> u >> v;
            u++, v++;
            ll s = 0;
            hld.path(u, v, [&](int l, int r) { s += bit.range(l + 1, r + 1); });
            cout << s << '\n';
        }
    }
}
