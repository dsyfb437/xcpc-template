/**
 * @brief 阶乘去掉素因子后取模
 * @complexity 预处理 O(p)，单次 O(log_p n)
 * @index 非负 n
 * @idea 按长度 p 分块，完整一块去掉 p 的倍数后乘积为 -1。再递归处理 p 的倍数除以 p 的部分。
 * @usage PrimeFactorial fact(p); auto [unit, exponent] = fact.get(n); 得到 n! 去掉全部 p 因子后的余数和 p 的次数。
 * @pitfall p 为素数，需 O(p) 空间。普通 n! mod p 在 exponent 非零时为 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct PrimeFactorial {
    int p;
    vector<int> prefix;

    explicit PrimeFactorial(int p) : p(p), prefix(p, 1) {
        for (int i = 1; i < p; i++)
            prefix[i] = 1LL * prefix[i - 1] * i % p;
    }

    pair<int, ull> get(ull n) const {
        ll unit = 1;
        ull exponent = 0;
        while (n) {
            unit = unit * prefix[n % p] % p;
            n /= p;
            exponent += n;
            if (n & 1)
                unit = p - unit;
        }
        return {(int)unit, exponent};
    }
};
