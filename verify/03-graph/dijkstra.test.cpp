#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include <bits/stdc++.h>
#include "../../alg/03-graph/dijkstra.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<pair<int, ll>>> g(n + 1), rev(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        g[a + 1].push_back({b + 1, c});
        rev[b + 1].push_back({a + 1, c});
    }
    auto d = dijkstra(g, s + 1);
    if (d[t + 1] == INF) {
        cout << -1 << '\n';
        return 0;
    }
    // 零权 dense 图里可能有自环/等距环，重建时跳过走过的点，防止原地打转。
    vector<pair<int, int>> path;
    vector<char> seen(n + 1, 0);
    for (int cur = t + 1; cur != s + 1;) {
        seen[cur] = 1;
        int next = 0;
        for (auto [v, w] : rev[cur])
            if (!seen[v] && d[v] + w == d[cur]) {
                next = v;
                break;
            }
        if (!next) break;
        path.push_back({next - 1, cur - 1});
        cur = next;
    }
    reverse(path.begin(), path.end());
    cout << d[t + 1] << ' ' << path.size() << '\n';
    for (auto [u, v] : path) cout << u << ' ' << v << '\n';
}
