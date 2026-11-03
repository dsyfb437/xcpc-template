/**
 * @brief 有理数连分数与渐近分数
 * @complexity O(log max(p, q))
 * @index 第 0 项是整数部分
 * @idea 辗转相除得到连分数各项。分子和分母分别用前两项渐近分数递推。
 * @usage continued_fraction(p, q) 返回部分商，convergents(terms) 返回各前缀的最简分数。
 * @pitfall p 非负，q 为正。convergents 的输入来自普通有限连分数，分子分母须放进 uint64_t。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline vector<ull> continued_fraction(ull p, ull q) {
    vector<ull> terms;
    while (q) {
        terms.push_back(p / q);
        ull rest = p % q;
        p = q;
        q = rest;
    }
    return terms;
}

inline vector<pair<ull, ull>> convergents(const vector<ull> &terms) {
    ull p0 = 0, p1 = 1, q0 = 1, q1 = 0;
    vector<pair<ull, ull>> result;
    for (ull a : terms) {
        ull p = a * p1 + p0, q = a * q1 + q0;
        result.emplace_back(p, q);
        p0 = p1;
        q0 = q1;
        p1 = p;
        q1 = q;
    }
    return result;
}
