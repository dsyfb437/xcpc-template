#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"

#include <bits/stdc++.h>
#include "../../alg/03-graph/kruskal.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        edges[i] = {a + 1, b + 1, i, c};
    }
    auto [cost, ids] = kruskal(n, edges);
    cout << cost << '\n';
    for (int i = 0; i < (int)ids.size(); i++)
        cout << ids[i] << " \n"[i + 1 == (int)ids.size()];
}
