/**
 * @brief 扩展 Lucas
 * @complexity 试除 O(sqrt(m))，各素数幂 q 预处理 O(q)
 * @index 非负 n、k
 * @idea 模数拆成素数幂。阶乘中单独统计 p 的次数，剩下的部分可以求逆，最后用 CRT 合并。
 * @usage extended_lucas(n, k, mod) 求 C(n, k) mod mod。
 * @pitfall mod 是正 int，且每个素数幂因子 q 都能开 O(q) 的表。单个因子建表后计算 O(log_p n + log q)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline int extended_lucas(ull n, ull k, int mod) {
    if (k > n || mod == 1)
        return 0;

    auto prime_power = [&](int p, int q, int exponent) -> int {
        auto valuation = [&](ull x) {
            ull count = 0;
            while (x) {
                x /= p;
                count += x;
            }
            return count;
        };
        auto e = valuation(n) - valuation(k) - valuation(n - k);
        if (e >= (unsigned)exponent)
            return 0;
        auto power = [&](ll a, ull b) {
            ll result = 1;
            for (; b; b >>= 1, a = a * a % q)
                if (b & 1)
                    result = result * a % q;
            return result;
        };

        vector<int> product(q, 1);
        for (int i = 1; i < q; i++)
            product[i] = i % p ? 1LL * product[i - 1] * i % q : product[i - 1];
        auto unit = [&](ull x) {
            ll result = 1;
            while (x) {
                result = result * product[x % q] % q;
                // 一个完整周期的乘积只有 1 或 -1。
                if (x / q % 2)
                    result = result * product.back() % q;
                x /= p;
            }
            return result;
        };
        ll bottom = unit(k) * unit(n - k) % q;
        int phi = q - q / p;
        return unit(n) * power(bottom, phi - 1) % q * power(p, e) % q;
    };

    ll answer = 0, combined = 1;
    auto merge = [&](int p, int q, int exponent) {
        ll a = combined % q, b = q, x = 1, y = 0;
        while (b) {
            ll quotient = a / b;
            ll r = a % b, z = x - quotient * y;
            a = b;
            b = r;
            x = y;
            y = z;
        }
        ll step = (prime_power(p, q, exponent) - answer) * x % q;
        if (step < 0)
            step += q;
        answer += combined * step;
        combined *= q;
    };

    int remaining = mod;
    for (int p = 2; p <= remaining / p; p++) {
        if (remaining % p)
            continue;
        int q = 1, exponent = 0;
        while (remaining % p == 0) {
            remaining /= p;
            q *= p;
            exponent++;
        }
        merge(p, q, exponent);
    }
    if (remaining > 1)
        merge(remaining, remaining, 1);
    return (int)answer;
}
