/**
 * @brief 反 Nim（拿最后一颗就输）
 * @complexity O(n)
 * @index 石堆从 0 编号
 * @idea 有一堆至少两颗时按普通 Nim 判。否则，非空堆数为偶数时先手赢。
 * @usage bool win = misere_nim_first_wins(piles);
 * @pitfall 允许空堆。石子全部取空后，轮到的人赢。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline bool misere_nim_first_wins(const vector<ll> &piles) {
    ll sum = 0;
    bool has_large_pile = false;
    for (ll stones : piles) {
        sum ^= stones;
        has_large_pile |= stones > 1;
    }
    return has_large_pile ? sum != 0 : sum == 0;
}
