/**
 * @brief 快速沃尔什变换（AND / OR / XOR）
 * @complexity O(n log n)
 * @index 数组下标为位集
 * @idea 逐位做蝶形变换，把按位运算卷积变成对应位置的乘积。逆变换逐层还原。
 * @usage bitwise_convolution(a,b,FwtOp::xor_op)，另外有 and_op 和 or_op。
 * @pitfall 数组同长且长度为 2 的幂，系数先归约到 [0,998244353)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

enum class FwtOp { and_op, or_op, xor_op };

inline void fwt(vector<int> &a, FwtOp op, bool inverse) {
    constexpr int mod = 998244353, half = (mod + 1) / 2;
    int n = (int)a.size();
    assert(has_single_bit((unsigned)n));
    for (int len = 1; len < n; len *= 2)
        for (int i = 0; i < n; i += 2 * len)
            for (int j = 0; j < len; j++) {
                int &x = a[i + j], &y = a[i + j + len];
                if (op == FwtOp::and_op) {
                    x = (x + (inverse ? mod - y : y)) % mod;
                } else if (op == FwtOp::or_op) {
                    y = (y + (inverse ? mod - x : x)) % mod;
                } else {
                    int sum = (x + y) % mod, difference = (x - y + mod) % mod;
                    x = inverse ? 1LL * sum * half % mod : sum;
                    y = inverse ? 1LL * difference * half % mod : difference;
                }
            }
}

inline vector<int> bitwise_convolution(vector<int> a, vector<int> b, FwtOp op) {
    constexpr int mod = 998244353;
    assert(a.size() == b.size());
    fwt(a, op, false);
    fwt(b, op, false);
    for (int i = 0; i < (int)a.size(); i++)
        a[i] = 1LL * a[i] * b[i] % mod;
    fwt(a, op, true);
    return a;
}
