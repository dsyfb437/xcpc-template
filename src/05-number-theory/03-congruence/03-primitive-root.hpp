/**
 * @brief 素数模原根
 * @complexity 试除 O(sqrt(p))，每个候选做 O(omega(p - 1)) 次快速幂
 * @index 根在 1 到 p - 1
 * @idea 分解 p - 1。若对每个不同质因子 q，g^((p-1)/q) 都不等于 1，则 g 的阶是 p - 1。
 * @usage primitive_root_prime(p) 返回最小原根。
 * @pitfall p 必须是素数，候选从小到大枚举。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline int primitive_root_prime(int p) {
    if (p == 2)
        return 1;
    vector<int> divisors;
    int rest = p - 1;
    for (int q = 2; q <= rest / q; q++) {
        if (rest % q)
            continue;
        divisors.push_back(q);
        do {
            rest /= q;
        } while (rest % q == 0);
    }
    if (rest > 1)
        divisors.push_back(rest);

    auto power = [p](ll a, int e) {
        ll result = 1;
        for (; e; e >>= 1, a = a * a % p)
            if (e & 1)
                result = result * a % p;
        return result;
    };
    for (int g = 2; g < p; g++) {
        bool ok = true;
        for (int q : divisors)
            if (power(g, (p - 1) / q) == 1) {
                ok = false;
                break;
            }
        if (ok)
            return g;
    }
    return -1;
}
