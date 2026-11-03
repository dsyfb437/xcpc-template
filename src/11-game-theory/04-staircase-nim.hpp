/**
 * @brief 阶梯 Nim
 * @complexity O(n)
 * @index a[0] 是地面，a[1] 是第一级台阶
 * @idea 每次选一级，把任意正数颗石子搬到下一级。搬到地面就不能再动。异或所有奇数级的石子数。
 * @usage bool win = staircase_first_wins(stones);
 * @pitfall 若读入只包含台阶 1 到 n，先在开头补一个 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline bool staircase_first_wins(const vector<ll> &stones) {
    ll sum = 0;
    for (size_t i = 1; i < stones.size(); i += 2)
        sum ^= stones[i];
    return sum != 0;
}
