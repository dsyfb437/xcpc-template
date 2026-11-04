/**
 * @brief 实数二分
 * @complexity O(100)
 * 
 * @idea check 单调时，在实数区间 [lo, hi] 上找边界，固定迭代 100 次。
 * check 的单调性：real_first_true 先假后真，real_last_true 先真后假。
 * @usage `ld x = real_first_true(0, 1e9, [&](ld m) { return m * m >= v; });`
 * @pitfall 端点有限且 lo ≤ hi。精度受初始区间宽度和浮点舍入限制，端点很大时不能保证 1e-12 的绝对误差。
 * 
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

// 第一个满足 check 的位置（近似）；要求 check(hi) 为真。
template <class F> ld real_first_true(ld lo, ld hi, F check) {
    for (int i = 0; i < 100; i++) {
        ld mid = midpoint(lo, hi);
        if (check(mid))
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

// 最后一个满足 check 的位置（近似）；要求 check(lo) 为真。
template <class F> ld real_last_true(ld lo, ld hi, F check) {
    for (int i = 0; i < 100; i++) {
        ld mid = midpoint(lo, hi);
        if (check(mid))
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}
