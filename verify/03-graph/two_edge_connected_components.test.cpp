#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"

#include <bits/stdc++.h>
#include "../../alg/03-graph/bridges.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> g(n + 1);
    vector<int> eu(m), ev(m);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a++, b++;
        eu[i] = a, ev[i] = b;
        g[a].push_back({b, i});
        g[b].push_back({a, i});
    }
    Bridges br(g, m);

    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    auto find = [&](int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    };
    for (int i = 0; i < m; i++)
        if (!br.bridge[i]) {
            int a = find(eu[i]), b = find(ev[i]);
            if (a != b)
                p[a] = b;
        }

    vector<vector<int>> groups(n + 1);
    int k = 0;
    for (int v = 1; v <= n; v++) {
        int r = find(v);
        if (groups[r].empty())
            k++;
        groups[r].push_back(v - 1);
    }
    cout << k << '\n';
    for (int v = 1; v <= n; v++) {
        if (groups[v].empty())
            continue;
        cout << groups[v].size();
        for (int x : groups[v])
            cout << ' ' << x;
        cout << '\n';
    }
}
