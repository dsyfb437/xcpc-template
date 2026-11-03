/**
 * @brief 同余最短路
 * @complexity O(a m log(a m + 1))，a 是最小面值，m 是面值个数
 * @index 余数 0 到 a - 1
 *
 * @idea 每个余数是一个点，加一个面值就是走一条边。
 *       dist[r] 是能凑出且余数为 r 的最小金额，再加任意个 a 仍然能凑出。
 * @usage auto dist = residue_shortest(coins);
 *        金额 x 非负时，用 dist[x % dist.size()] <= x 判断能否凑出。
 * @pitfall 面值非空、为正且不限用量，最小面值不能太大。
 *          不可达为 RESIDUE_INF，查询金额须小于这个值。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline constexpr ll RESIDUE_INF = 1LL << 62;

inline vector<ll> residue_shortest(const vector<int> &coins) {
    int mod = *min_element(coins.begin(), coins.end());
    vector<ll> dist(mod, RESIDUE_INF);
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[0] = 0;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [d, r] = pq.top();
        pq.pop();
        if (d != dist[r])
            continue;
        for (int coin : coins) {
            int next = (r + 1LL * coin) % mod;
            if (d + coin >= dist[next])
                continue;
            dist[next] = d + coin;
            pq.emplace(dist[next], next);
        }
    }
    return dist;
}
