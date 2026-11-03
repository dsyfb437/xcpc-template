/**
 * @brief 二分图最大权完美匹配（KM）
 * @complexity O(n^3)
 * @index 两边均 0-based
 * @idea 用匈牙利算法对边权取反做最小费用指派。势能维护非负约化费用，每轮沿最短增广路加一对。
 * @usage auto [weight, match] = km(w); match[u] 是左点 u 匹配的右点。
 * @pitfall 要求完整方阵，允许负权。缺边或允许不匹配时按题意补边和虚点，权值及势能运算须避免溢出。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<ll, vector<int>> km(const vector<vector<ll>> &w) {
    int n = (int)w.size();
    vector<ll> left(n + 1), right(n + 1);
    vector<int> owner(n + 1), previous(n + 1);
    constexpr ll inf = numeric_limits<ll>::max();

    for (int u = 1; u <= n; u++) {
        owner[0] = u;
        int j = 0;
        vector<ll> slack(n + 1, inf);
        vector<bool> used(n + 1);
        do {
            used[j] = true;
            int i = owner[j], next = 0;
            ll delta = inf;
            for (int v = 1; v <= n; v++) {
                if (used[v])
                    continue;
                ll cost = -w[i - 1][v - 1] - left[i] - right[v];
                if (cost < slack[v]) {
                    slack[v] = cost;
                    previous[v] = j;
                }
                if (slack[v] < delta) {
                    delta = slack[v];
                    next = v;
                }
            }
            for (int v = 0; v <= n; v++) {
                if (used[v]) {
                    left[owner[v]] += delta;
                    right[v] -= delta;
                } else {
                    slack[v] -= delta;
                }
            }
            j = next;
        } while (owner[j] != 0);

        while (j != 0) {
            owner[j] = owner[previous[j]];
            j = previous[j];
        }
    }

    vector<int> match(n);
    ll weight = 0;
    for (int v = 1; v <= n; v++) {
        int u = owner[v] - 1;
        match[u] = v - 1;
        weight += w[u][v - 1];
    }
    return {weight, move(match)};
}
