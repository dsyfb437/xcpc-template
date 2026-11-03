/**
 * @brief 升幂引理（LTE）
 * @complexity O(log_p max(a, b, n))
 * @index 正整数 a、b、n
 * @idea 奇素数 p 下把 v_p(a^n - b^n) 拆成 v_p(a - b) + v_p(n)。p = 2 且 n 偶数时还要加 v_2(a + b) - 1。
 * @usage lte_difference(a, b, n, p) 返回 a^n - b^n 的 p 进赋值。
 * @pitfall p 为素数，a != b，p 整除 a - b 且不整除 a、b。调用方保证 p 为素数，其余条件不成立时返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline optional<int> lte_difference(ull a, ull b, ull n, ull p) {
    ull diff = a > b ? a - b : b - a;
    if (!diff || !n || p < 2 || diff % p || a % p == 0)
        return nullopt;
    auto valuation = [p](ull x) {
        int count = 0;
        while (x % p == 0) {
            x /= p;
            count++;
        }
        return count;
    };
    int result = valuation(diff);
    if (p == 2 && n % 2 == 0)
        result += countr_zero(a + b) - 1;
    return result + valuation(n);
}
