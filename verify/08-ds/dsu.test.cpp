#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

#include <bits/stdc++.h>
#include "../../alg/08-ds/dsu.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    DSU dsu(n);
    while (q--) {
        int t, u, v;
        cin >> t >> u >> v;
        if (t == 0)
            dsu.unite(u + 1, v + 1);
        else
            cout << (dsu.same(u + 1, v + 1) ? 1 : 0) << '\n';
    }
}
