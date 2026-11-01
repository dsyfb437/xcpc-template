/**
 * @brief 整数二分
 * @complexity O(log n)
 *
 * 用途：在单调区间 [lo, hi] 上找第一个（first_true）或最后一个（last_true）
 * 满足 check 的位置。比手写 while 少两个边界错误。
 *
 * check 必须单调：first_true 要求先全 false 后全 true，
 * last_true 要求先全 true 后全 false。区间内必须至少有一个满足的位置，
 * 否则返回值没有意义（不会报错，也不返回 -1）。
 *
 * 例：`first_true(0, 1e9, [&](ll x) { return x * x >= n; })`
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 最小的 x，使 check(x) 为 true；要求 check(hi) 为 true。
template <class F> ll first_true(ll lo, ll hi, F check) {
    while (lo < hi) {
        ll mid = lo + ((hi - lo) >> 1);
        if (check(mid))
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

// 最大的 x，使 check(x) 为 true；要求 check(lo) 为 true。
template <class F> ll last_true(ll lo, ll hi, F check) {
    while (lo < hi) {
        ll mid = lo + ((hi - lo + 1) >> 1); // 上取整，否则会死循环
        if (check(mid))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}
