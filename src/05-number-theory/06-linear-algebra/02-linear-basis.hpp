/**
 * @brief 线性基（异或）
 * @complexity O(63) 每次插入/查询
 * @pitfall 只接受非负 long long，处理第 0 到第 62 位。
 *
 * 维护一组数的异或空间：插入若干数后，能异或出的最大值就是 max_xor()。
 * 例：
 *   LinearBasis lb;
 *   lb.insert(x);
 *   ll mx = lb.max_xor();
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LinearBasis {
    static constexpr int B = 63;
    ll d[B] = {};

    // 插入 x；已经能被表示时返回 false
    bool insert(ll x) {
        for (int i = B - 1; i >= 0; i--) {
            if (!(x >> i & 1))
                continue;
            if (!d[i]) {
                d[i] = x;
                return true;
            }
            x ^= d[i];
        }
        return false;
    }

    // 能异或出的最大值
    ll max_xor() const {
        ll r = 0;
        for (int i = B - 1; i >= 0; i--)
            r = max(r, r ^ d[i]);
        return r;
    }
};
