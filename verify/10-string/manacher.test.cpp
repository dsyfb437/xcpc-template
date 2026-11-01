#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_palindromes"

#include <bits/stdc++.h>
#include "../../alg/10-string/manacher.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto [d1, d2] = manacher(s);
    int n = s.size();
    vector<int> len(2 * n - 1);
    for (int i = 0; i < n; i++)
        len[2 * i] = 2 * d1[i] - 1;
    for (int i = 0; i + 1 < n; i++)
        len[2 * i + 1] = 2 * d2[i + 1];
    for (int i = 0; i < 2 * n - 1; i++)
        cout << len[i] << " \n"[i + 1 == 2 * n - 1];
}
