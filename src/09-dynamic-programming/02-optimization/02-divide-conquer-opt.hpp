/**
 * @brief 分治优化（分段 DP）
 * @complexity O(k n log n)，假设 cost O(1)
 * @index 位置 0 到 n，段为 [l,r)
 * @idea 固定一层，先求中间位置的最佳分点。最优分点单调时，左右半边只需各搜一半候选范围。
 * @usage divide_conquer_partition(n,k,cost) 把 n 项分成 k 个非空连续段，最小化 cost(l,r) 之和。
 * @pitfall cost 返回 long long。每层取最小最优分点时必须随 r 单调不减。段数非法返回 nullopt。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Cost>
optional<__int128> divide_conquer_partition(int n, int k, Cost cost) {
    if (k < 0 || k > n || (!k && n))
        return nullopt;
    using Wide = __int128;
    constexpr Wide inf = (Wide)1 << 120;
    vector<Wide> previous(n + 1, inf), current(n + 1, inf);
    previous[0] = 0;
    for (int group = 1; group <= k; group++) {
        auto solve = [&](auto &&self, int l, int r, int low, int high) -> void {
            if (l >= r)
                return;
            int mid = (l + r) / 2, best_at = low;
            Wide best = inf;
            for (int p = low; p <= min(high, mid - 1); p++) {
                if (previous[p] == inf)
                    continue;
                Wide candidate = previous[p] + cost(p, mid);
                if (candidate < best) {
                    best = candidate;
                    best_at = p;
                }
            }
            current[mid] = best;
            self(self, l, mid, low, best_at);
            self(self, mid + 1, r, best_at, high);
        };
        fill(current.begin(), current.end(), inf);
        solve(solve, group, n + 1, group - 1, n - 1);
        swap(previous, current);
    }
    return previous[n];
}
