/**
 * @brief 随机数
 * @complexity -
 *
 * 现场赛别用 rand()（Windows 下 RAND_MAX 只有 32767，固定种子还会被卡）。
 * 需要复现就把种子写死。
 *
 * 例：
 *   ll x = rand_ll(1, 100);
 *   double y = rand_real(0, 1); */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rand_ll(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
}

double rand_real(double l, double r) {
    return uniform_real_distribution<double>(l, r)(rng);
}
