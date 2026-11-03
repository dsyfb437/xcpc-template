/**
 * @brief 有限域的二次扩张
 * @complexity 乘法 O(1)，幂 O(log e)
 * @index 元素用 {a, b} 表示 a + b sqrt(d)
 * @idea 用 sqrt(d)^2 = d 做乘法。求逆时乘共轭，再除以范数 a^2 - d b^2。
 * @usage QuadraticField f(p, d); f.multiply(x, y)、f.power(x, e)、f.inverse(x)。
 * @pitfall p 是奇素数，d 是模 p 的二次非剩余。系数和 d 先归约到 [0, p)，零元不可求逆。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct QuadraticField {
    using Value = pair<int, int>;
    int p, d;

    QuadraticField(int p, int d) : p(p), d(d) {}

    Value multiply(Value x, Value y) const {
        auto [a, b] = x;
        auto [c, e] = y;
        ll real = (1LL * a * c % p + 1LL * b * e % p * d % p) % p;
        ll imag = (1LL * a * e + 1LL * b * c) % p;
        return {(int)real, (int)imag};
    }

    Value power(Value x, ull e) const {
        Value result{1, 0};
        for (; e; e >>= 1, x = multiply(x, x))
            if (e & 1)
                result = multiply(result, x);
        return result;
    }

    Value inverse(Value x) const {
        auto [a, b] = x;
        Value conjugate{a, b == 0 ? 0 : p - b};
        auto [norm, zero] = multiply(x, conjugate);
        int inv = power({norm, 0}, p - 2).first;
        return multiply(conjugate, {inv, 0});
    }
};
