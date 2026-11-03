/**
 * @brief bitset 优化（子集和）
 * @complexity O(n·ceil((MaxSum+1) / 机器字长)) 次字操作
 * @index 第 s 位表示能不能凑出和 s
 * @idea 初始只能凑出 0。读到一个数 x，就把原有可达的和都加上 x，再并回去。
 * @usage auto can = subset_sums<100000>(a);
 *   bool reachable = can.test(target);  // 0 ≤ target ≤ 100000
 * @pitfall MaxSum 编译时确定。a 里的数要非负，每个数最多用一次。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <size_t MaxSum> bitset<MaxSum + 1> subset_sums(const vector<int> &a) {
    bitset<MaxSum + 1> can;
    can.set(0);
    for (int x : a)
        can |= can << x;
    return can;
}
