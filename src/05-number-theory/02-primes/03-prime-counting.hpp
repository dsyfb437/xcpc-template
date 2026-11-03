/**
 * @brief 素数计数（整除分块筛）
 * @complexity O(n^(3/4))，空间 O(sqrt(n))
 * @index 统计 1 到 n
 * @idea 只维护 x <= sqrt(n) 和 x = floor(n / i) 的筛值。遇到质数 p 时扣掉最小质因子恰为 p 的合数。
 * @usage prime_count(n) 返回不超过 n 的质数个数。
 * @pitfall n 为非负整数，需要两张 sqrt(n) 长度的表。特别大的 n 要留意时间和内存。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;

inline ull prime_count(ull n) {
    if (n < 2)
        return 0;
    ull root = (ull)sqrt((ld)n);
    while (root + 1 <= n / (root + 1))
        root++;
    while (root > n / root)
        root--;

    vector<ull> small(root + 1), large(root + 1);
    for (ull i = 1; i <= root; i++) {
        small[i] = i - 1;
        large[i] = n / i - 1;
    }
    for (ull p = 2; p <= root; p++) {
        if (small[p] == small[p - 1])
            continue;
        ull before = small[p - 1], square = p * p;
        ull limit = min(root, n / square);
        for (ull i = 1; i <= limit; i++) {
            ull x = n / i / p;
            ull count = x <= root ? small[x] : large[i * p];
            large[i] -= count - before;
        }
        for (ull x = root; x >= square; x--)
            small[x] -= small[x / p] - before;
    }
    return large[1];
}
