/**
 * @brief WQS 二分（恰好选 k 个）
 * @complexity O(T log V)，T 是带罚项求解代价
 * @index 选取数 k 非负
 * @idea 给每个被选对象扣同样的罚值。罚值越大，最优选取数越少。二分跨过 k 的临界点，再补回 k 份罚值。
 * @usage wqs_max_exact(k,low,high,solve)；solve(p) 返回 {扣罚后的最优值,选取数}，同值选数较多。
 * @pitfall 恰选数的最优值序列须离散凹、有限可行域连续。low 的选数 >= k，high 的选数 <= k，否则返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Solve>
optional<__int128> wqs_max_exact(int k, ll low, ll high, Solve solve) {
    if (low > high || solve(low).second < k || solve(high).second > k)
        return nullopt;
    while (low < high) {
        ll mid = midpoint(high, low);
        if (solve(mid).second >= k)
            low = mid;
        else
            high = mid - 1;
    }
    return (__int128)solve(low).first + (__int128)low * k;
}
