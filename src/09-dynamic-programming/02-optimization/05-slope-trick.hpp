/**
 * @brief Slope Trick（凸分段线性函数）
 * @complexity 加折线 O(log n)，平移和取最小 O(1)
 * @index 初始 f(x)=0
 * @idea 用左右两个堆记斜率变化点。加 max(x-a,0) 或 max(a-x,0) 时搬移堆顶，同时修正最小值。
 * @usage SlopeTrick f; f.add_abs(a); f.add_x_minus(a); f.add_minus_x(a); f.shift(a,b); f.minimum()。
 * @pitfall shift(a,b) 要求 a<=b，把 f 改成 min_{x-b<=y<=x-a} f(y)。所有坐标和偏移运算须放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SlopeTrick {
    priority_queue<ll> left;
    priority_queue<ll, vector<ll>, greater<ll>> right;
    ll left_shift = 0, right_shift = 0;
    __int128 best = 0;

    void add_x_minus(ll a) {
        left.push(a - left_shift);
        ll x = left.top() + left_shift;
        left.pop();
        best += (__int128)x - a;
        right.push(x - right_shift);
    }

    void add_minus_x(ll a) {
        right.push(a - right_shift);
        ll x = right.top() + right_shift;
        right.pop();
        best += (__int128)a - x;
        left.push(x - left_shift);
    }

    void add_abs(ll a) {
        add_x_minus(a);
        add_minus_x(a);
    }

    void shift(ll a, ll b) {
        left_shift += a;
        right_shift += b;
    }

    __int128 minimum() const { return best; }
};
