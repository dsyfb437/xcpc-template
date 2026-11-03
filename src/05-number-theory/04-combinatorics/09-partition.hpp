/**
 * @brief 整数分拆数
 * @complexity O(n sqrt(n))
 * @index 下标 0 到 n
 * @idea Euler 五边形数递推。偏移 k(3k ± 1)/2，每两项同号，符号按加加减减循环。
 * @usage partition_table(n, mod) 返回各整数拆成无序正整数和的方案数。
 * @pitfall mod 为正 int，可为合数。0 的空分拆算一种。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> partition_table(int n, int mod) {
    vector<int> p(n + 1);
    p[0] = 1 % mod;
    for (int i = 1; i <= n; i++) {
        ll count = 0;
        for (ll k = 1;; k++) {
            ll a = k * (3 * k - 1) / 2;
            if (a > i)
                break;
            ll b = a + k;
            ll term = p[i - a];
            if (b <= i)
                term += p[i - b];
            count += k & 1 ? term : -term;
        }
        p[i] = (count % mod + mod) % mod;
    }
    return p;
}
