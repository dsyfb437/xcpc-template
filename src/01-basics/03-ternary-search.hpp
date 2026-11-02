/**
 * @brief 三分
 * @complexity O(log n · f) 或者 O(100 · f)
 * @idea 单峰函数求最大值，f 先增后减。求最小值就把比较反过来。整数版最后剩 3 个点暴力扫；实数版迭代 100 次。
 * @usage 例：`ll pos = ternary_max(0, n, [&](ll x) { return f(x); });`
 * @pitfall 函数有平台（连续相等）时三分可能失效，用之前先确认严格单峰。
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

// 单峰最大值的位置；f 先增后减，返回值是位置，极值是 f(位置)。
template <class F> ll ternary_max(ll lo, ll hi, F f) {
    while (hi - lo > 3) {
        ll m1 = lo + (hi - lo) / 3;
        ll m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2))
            lo = m1;
        else
            hi = m2;
    }
    ll best = lo;
    for (ll x = lo + 1; x <= hi; x++)
        if (f(x) > f(best))
            best = x;
    return best;
}

// 实数单峰最大值的横坐标。
template <class F> ld ternary_max_real(ld lo, ld hi, F f) {
    for (int i = 0; i < 100; i++) {
        ld m1 = lo + (hi - lo) / 3;
        ld m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2))
            lo = m1;
        else
            hi = m2;
    }
    return midpoint(lo, hi);
}
