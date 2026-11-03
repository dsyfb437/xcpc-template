/**
 * @brief Pólya 计数（旋转项链）
 * @complexity O(sqrt(n) + d(n) log n)
 * @index n 个珠子
 * @idea 旋转的循环数由 gcd 决定。按约数 d 合并贡献 phi(d) * colors^(n/d)，最后除以 n。
 * @usage rotation_necklaces(n, colors, mod) 返回只把旋转视作相同的染色数。
 * @pitfall n、mod 为正 int，colors 非负。先模 n * mod 再整除，因此 mod 可以与 n 不互质。需 128 位乘法。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline int rotation_necklaces(int n, ull colors, int mod) {
    vector<pair<int, int>> factors;
    int rest = n;
    for (int p = 2; p <= rest / p; p++) {
        if (rest % p)
            continue;
        int exponent = 0;
        do {
            rest /= p;
            exponent++;
        } while (rest % p == 0);
        factors.emplace_back(p, exponent);
    }
    if (rest > 1)
        factors.emplace_back(rest, 1);

    ull lifted = 1ULL * n * mod;
    auto multiply = [&](ull a, ull b) {
        return (ull)((unsigned __int128)a * b % lifted);
    };
    auto power = [&](int e) {
        ull answer = 1 % lifted, a = colors % lifted;
        for (; e; e >>= 1, a = multiply(a, a))
            if (e & 1)
                answer = multiply(answer, a);
        return answer;
    };
    ull sum = 0;
    auto dfs = [&](auto &&self, int i, int divisor, int phi) -> void {
        if (i == (int)factors.size()) {
            sum = (sum + multiply(phi, power(n / divisor))) % lifted;
            return;
        }
        auto [p, exponent] = factors[i];
        self(self, i + 1, divisor, phi);
        for (int e = 1; e <= exponent; e++) {
            divisor *= p;
            phi *= e == 1 ? p - 1 : p;
            self(self, i + 1, divisor, phi);
        }
    };
    dfs(dfs, 0, 1, 1);
    return (int)(sum / n);
}
