/**
 * @brief Nim 游戏
 * @complexity O(n)
 * @index 石堆从 0 编号
 * @idea 每次只能从一堆取走任意正数颗，拿走最后一颗的人赢。各堆异或为 0 时先手必败。
 * @usage auto move = nim_move(piles);
 *   有解时返回 {堆编号, 取完后剩几颗}。nullopt 表示必败。
 * @pitfall 每堆石子数非负。返回的是剩余数量。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<pair<int, ll>> nim_move(const vector<ll> &piles) {
    ll sum = 0;
    for (ll stones : piles)
        sum ^= stones;
    if (sum == 0)
        return nullopt;

    for (int i = 0; i < (int)piles.size(); i++) {
        ll remaining = piles[i] ^ sum;
        if (remaining < piles[i])
            return pair{i, remaining};
    }
    return nullopt;
}
