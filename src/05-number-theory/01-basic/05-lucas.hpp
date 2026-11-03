/**
 * @brief Lucas 定理
 * @complexity 预处理 O(p + log p)，单次 O(log_p n)
 * @index 非负 n、k
 * @idea 把 n、k 按 p 进制拆开。每一位的组合数相乘，某一位 k 大于 n 时结果为 0。
 * @usage Lucas lucas(p); 用 lucas.choose(n, k) 求 C(n, k) mod p。
 * @pitfall p 必须为素数，且能开 O(p) 的数组。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct Lucas {
    int p;
    vector<int> fact, inverse_fact;

    explicit Lucas(int p) : p(p), fact(p, 1), inverse_fact(p, 1) {
        for (int i = 1; i < p; i++)
            fact[i] = 1LL * fact[i - 1] * i % p;
        ll inv = 1, base = fact.back();
        for (int e = p - 2; e; e >>= 1, base = base * base % p)
            if (e & 1)
                inv = inv * base % p;
        inverse_fact.back() = inv;
        for (int i = p - 1; i > 0; i--)
            inverse_fact[i - 1] = 1LL * inverse_fact[i] * i % p;
    }

    int choose(ull n, ull k) const {
        if (k > n)
            return 0;
        ll answer = 1;
        while (k) {
            int a = n % p, b = k % p;
            if (b > a)
                return 0;
            answer = answer * fact[a] % p * inverse_fact[b] % p;
            answer = answer * inverse_fact[a - b] % p;
            n /= p;
            k /= p;
        }
        return answer;
    }
};
