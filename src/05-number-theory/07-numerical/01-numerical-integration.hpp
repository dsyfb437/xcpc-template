/**
 * @brief 自适应 Simpson 积分
 * @complexity 按细分区间数计
 * @index 闭区间 [l, r]
 * @idea 比较整段 Simpson 估计和分成两半后的估计。差距超限时继续细分，每半段分一半误差预算。
 * @usage adaptive_simpson(f, l, r, eps) 返回定积分近似值。
 * @pitfall 适合光滑实函数，eps 为正；端点、采样值和积分结果须有限。深度上限处直接返回近似值，尖峰、振荡和奇点需按题意分段。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

template <class F>
ld adaptive_simpson(F f, ld l, ld r, ld eps = 1e-10L, int max_depth = 24) {
    if (l == r)
        return 0;
    auto estimate = [](ld a, ld b, ld fa, ld fm, ld fb) {
        // 先求 Simpson 加权平均；区间长度溢出时改用半段长度。
        ld average = lerp(midpoint(fa, fb), fm, 2.L / 3);
        ld width = b - a;
        if (isfinite(width))
            return width * average;
        return (b / 2 - a / 2) * average * 2;
    };
    auto dfs = [&](auto &&self, ld a, ld b, ld fa, ld fm, ld fb, ld whole, ld tol,
                   int depth) -> ld {
        ld m = midpoint(a, b);
        if (m == a || m == b)
            return whole;
        ld fl = f(midpoint(a, m)), fr = f(midpoint(m, b));
        ld left = estimate(a, m, fa, fl, fm);
        ld right = estimate(m, b, fm, fr, fb);
        ld delta = left + right - whole;
        if (depth <= 0 || abs(delta) <= 15 * tol)
            return left + right + delta / 15;
        return self(self, a, m, fa, fl, fm, left, tol / 2, depth - 1) +
               self(self, m, b, fm, fr, fb, right, tol / 2, depth - 1);
    };
    ld fl = f(l), fm = f(midpoint(l, r)), fr = f(r);
    return dfs(dfs, l, r, fl, fm, fr, estimate(l, r, fl, fm, fr), eps, max_depth);
}
