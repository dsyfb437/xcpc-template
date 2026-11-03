/**
 * @brief 威佐夫博弈
 * @complexity O(1)
 * @index 两堆石子
 * @idea 一次可以从一堆拿若干颗，或从两堆拿同样多。设 a ≤ b，d = b-a，必败当且仅当 a = floor(d·φ)。
 * @usage bool win = wythoff_first_wins(a, b);
 * @pitfall 用整数比较代替浮点乘法，支持整个 uint64_t 范围。需要 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline bool wythoff_first_wins(ull a, ull b) {
    using u128 = unsigned __int128;
    if (a > b)
        swap(a, b);
    ull d = b - a;

    // φ 满足 φ² = φ + 1。先检查 a ≤ d·φ。
    u128 square = u128(a) * a, product = u128(d) * b;
    if (square > product)
        return true;

    // 再检查 a+1 > d·φ。用差值判断，避免算 (a+1)²。
    u128 increase = u128(a) * 2 + 1;
    bool cold = increase > d && product - square < increase - d;
    return !cold;
}
