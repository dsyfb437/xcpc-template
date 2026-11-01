#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include <bits/stdc++.h>
#include "../../alg/03-graph/tarjan_scc.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        g[a + 1].push_back(b + 1);
    }
    TarjanSCC scc(g);
    vector<vector<int>> group(scc.cnt);
    for (int v = 1; v <= n; v++)
        group[scc.comp[v]].push_back(v - 1);
    cout << scc.cnt << '\n';
    // Tarjan 的编号是反拓扑序，倒着输出就是拓扑序
    for (int id = scc.cnt - 1; id >= 0; id--) {
        cout << group[id].size();
        for (int v : group[id])
            cout << ' ' << v;
        cout << '\n';
    }
}
