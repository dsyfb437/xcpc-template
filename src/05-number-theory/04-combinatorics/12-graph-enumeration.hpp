/**
 * @brief 图论计数（带标号连通图）
 * @complexity O(n^2)，空间 O(n)
 * @index 下标 0 到 n
 * @idea 从所有图中扣掉不连通图。按包含 1 号点的连通块大小分类，块外的边任意选。
 * @usage connected_graph_counts(n, mod) 返回每个点数的连通简单无向图数量。
 * @pitfall mod 为正 int，可为合数。这里把空图的连通图数记为 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> connected_graph_counts(int n, int mod) {
    vector<int> total(n + 1), connected(n + 1), choose(n + 1);
    total[0] = choose[0] = 1 % mod;
    ll power = 1 % mod;
    for (int i = 1; i <= n; i++) {
        total[i] = total[i - 1] * power % mod;
        power = power * 2 % mod;
        ll count = total[i];
        for (int j = 1; j < i; j++) {
            count -= 1LL * choose[j - 1] * connected[j] % mod * total[i - j] % mod;
            if (count < 0)
                count += mod;
        }
        connected[i] = count;
        for (int j = i; j >= 1; j--)
            choose[j] = (choose[j] + (ll)choose[j - 1]) % mod;
    }
    return connected;
}
