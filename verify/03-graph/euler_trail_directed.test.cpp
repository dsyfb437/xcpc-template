#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_directed"

#include <bits/stdc++.h>
#include "../../alg/03-graph/euler_trail.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<vector<pair<int, int>>> g(n + 1);
        for (int i = 0; i < m; i++) {
            int a, b;
            cin >> a >> b;
            g[a + 1].push_back({b + 1, i});
        }
        auto res = euler_trail_directed(g, m);
        if (!res.ok) {
            cout << "No\n";
            continue;
        }
        cout << "Yes\n";
        for (int i = 0; i < (int)res.vs.size(); i++)
            cout << res.vs[i] - 1 << " \n"[i + 1 == (int)res.vs.size()];
        for (int i = 0; i < (int)res.es.size(); i++)
            cout << res.es[i] << " \n"[i + 1 == (int)res.es.size()];
    }
}
