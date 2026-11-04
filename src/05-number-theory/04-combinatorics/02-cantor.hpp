/**
 * @brief 康托展开 / 逆康托
 * @complexity O(n^2)
 * @index 排列值 1-based
 *
 * 把 1..n 的排列映射成字典序排名（0-based），或反过来。
 * 例：
 *   ll r = cantor_rank(p);          // p 是 1..n 的排列
 *   vector<int> q = cantor_unrank(n, r);
 * 坑：0 <= n <= 20（20! 还在 ll 范围内）；逆康托要求 0 <= r < n!。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<ll> factorial_table(int n) {
    vector<ll> fac(n + 1, 1);
    for (int i = 1; i <= n; i++)
        fac[i] = fac[i - 1] * i;
    return fac;
}

inline ll cantor_rank(const vector<int> &p) {
    int n = p.size();
    vector<ll> fac = factorial_table(n);
    ll r = 0;
    for (int i = 0; i < n; i++) {
        int cnt = 0;
        for (int j = i + 1; j < n; j++)
            if (p[j] < p[i])
                cnt++;
        r += cnt * fac[n - 1 - i];
    }
    return r;
}

inline vector<int> cantor_unrank(int n, ll r) {
    vector<ll> fac = factorial_table(n);
    vector<int> avail(n);
    iota(avail.begin(), avail.end(), 1);
    vector<int> p;
    p.reserve(n);
    for (int i = n; i >= 1; i--) {
        ll block = fac[i - 1];
        int idx = (int)(r / block);
        r %= block;
        p.push_back(avail[idx]);
        avail.erase(avail.begin() + idx);
    }
    return p;
}
