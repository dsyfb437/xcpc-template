/**
 * @brief 循环展开（四路求和）
 * @complexity O(n) 时间，O(1) 空间
 * @index 0-based
 * @idea 每轮处理四项，分别累加。最后单独处理不足四项的尾巴。
 * @usage long long sum = sum_unrolled(a);
 * @pitfall 部分和也要在 long long 范围内。编译器有时会自己展开，是否更快要实测。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline ll sum_unrolled(span<const int> a) {
    ll s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    size_t i = 0;
    for (; a.size() - i >= 4; i += 4) {
        s0 += a[i];
        s1 += a[i + 1];
        s2 += a[i + 2];
        s3 += a[i + 3];
    }
    for (; i < a.size(); i++)
        s0 += a[i];
    return s0 + s1 + s2 + s3;
}
