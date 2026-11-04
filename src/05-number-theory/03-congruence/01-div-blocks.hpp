/**
 * @brief 数论分块
 * @complexity O(sqrt n)
 * @pitfall n 非负。求和时答案可能超过 long long。
 *
 * 把 [1, n] 按 n / i 的取值分成 O(sqrt n) 段，每段内 n / i 相同。
 * 例：暴力求 sum_{i=1}^{n} n / i（会超时），用分块：
 *   __int128 s = 0;
 *   div_blocks(n, [&](ll l, ll r, ll q) { s += (r - l + 1) * q; });
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 对每一段 [l, r]（其中 n / i 恒等于 q）调用 f(l, r, q)
template <class F> void div_blocks(ll n, F f) {
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        f(l, r, q);
        if (r == n)
            break;
    }
}
