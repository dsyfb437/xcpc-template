#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include <bits/stdc++.h>
#include "../../alg/05-number/combination.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    ll m;
    cin >> t >> m;
    vector<pair<int, int>> qs(t);
    int mx = 0;
    for (auto &[n, k] : qs) {
        cin >> n >> k;
        mx = max(mx, n);
    }
    Comb comb(mx, m);
    for (auto [n, k] : qs)
        cout << comb.C(n, k) << '\n';
}
