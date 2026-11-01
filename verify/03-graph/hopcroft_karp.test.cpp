#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"

#include <bits/stdc++.h>
#include "../../alg/03-graph/hopcroft_karp.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int l, r, m;
    cin >> l >> r >> m;
    HopcroftKarp hk(l, r);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        hk.add_edge(a + 1, b + 1);
    }
    int k = hk.max_matching();
    cout << k << '\n';
    for (int u = 1; u <= l; u++)
        if (hk.match_l[u])
            cout << u - 1 << ' ' << hk.match_l[u] - 1 << '\n';
}
