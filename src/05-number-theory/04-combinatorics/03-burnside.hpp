/**
 * @brief Burnside 引理
 * @complexity O(g)，另计不动点统计
 * @index g 个群元素
 * @idea 每个群元素数出被它固定的对象，再取平均，就是轨道数。
 * @usage burnside_orbits(fixed) 返回轨道数，fixed[i] 是第 i 个群元素固定的对象数。
 * @pitfall 必须包含群里的每个元素，包括单位元。输入非空，传的是精确计数，需要 128 位整数累加。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline ull burnside_orbits(const vector<ull> &fixed) {
    unsigned __int128 sum = 0;
    for (ull count : fixed)
        sum += count;
    return (ull)(sum / fixed.size());
}
