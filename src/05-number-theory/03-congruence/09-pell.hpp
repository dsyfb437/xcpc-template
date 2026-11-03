/**
 * @brief Pell 方程
 * @complexity O(k)，k 是走过的连分数项数
 * @index 最小正整数解
 * @idea 逐项展开 sqrt(D) 的连分数，按顺序检查渐近分数，遇到第一个 x^2 - D*y^2 = 1 即停止。
 * @usage pell_minimum(D) 返回 {x, y}。D 是平方数或最小解超出 uint64_t 时返回 nullopt。
 * @pitfall D 为非负整数，只求正 Pell 方程。需要 unsigned __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;

inline optional<pair<ull, ull>> pell_minimum(ull D) {
    if (D < 2)
        return nullopt;
    ull root = (ull)sqrt((ld)D);
    while (root + 1 <= D / (root + 1))
        root++;
    while (root > D / root)
        root--;
    if (root * root == D)
        return nullopt;

    using Wide = unsigned __int128;
    ull m = 0, denom = 1, a = root;
    ull p0 = 0, p1 = 1, q0 = 1, q1 = 0;
    while (true) {
        Wide p = (Wide)a * p1 + p0, q = (Wide)a * q1 + q0;
        if (p > numeric_limits<ull>::max() || q > numeric_limits<ull>::max())
            return nullopt;
        Wide square = p * p;
        if (q && square / D == q * q && square % D == 1)
            return pair{(ull)p, (ull)q};
        p0 = p1;
        p1 = p;
        q0 = q1;
        q1 = q;
        m = denom * a - m;
        denom = (D - m * m) / denom;
        a = (root + m) / denom;
    }
}
