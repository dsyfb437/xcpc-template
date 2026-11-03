/**
 * @brief Kahan 求和
 * @complexity O(n) 时间，O(1) 空间
 * @index -
 * @idea 每次相加后，记住这一轮舍入丢掉的低位。下一轮先把它补回来。
 * @usage double total = kahan_sum(a);
 * @pitfall 输入和中间结果应为有限值。别开 -ffast-math，编译器可能把补偿项优化掉。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline double kahan_sum(span<const double> a) {
    double sum = 0, lost = 0;
    for (double x : a) {
        double corrected = x - lost;
        double next = sum + corrected;
        lost = (next - sum) - corrected;
        sum = next;
    }
    return sum;
}
