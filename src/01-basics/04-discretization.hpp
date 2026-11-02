/**
 * @brief 离散化
 * @complexity O(n log n)
 * @index 名次 1-based
 *
 * @idea 值域很大但只有 n 个不同值时，把值压成 1..k 的名次，方便开数组。
 *
 * 返回 rank，rank[i] 是 a[i] 的名次（1..k）；sorted 是去重排序后的值，
 * 满足 `sorted[rank[i] - 1] == a[i]`。原数组不动。
 * 不需要反查原值时用单参数版本；空数组返回空。
 *
 * @usage 例：`vector<ll> sorted; auto rank = discretize(a, sorted);  // rank[i] 在 1..k` 
 * @verified
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// rank[i] ∈ [1, k]，sorted[rank[i] - 1] == a[i]。
vector<int> discretize(const vector<ll> &a, vector<ll> &sorted) {
    sorted = a;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
    vector<int> rank(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        auto it = lower_bound(sorted.begin(), sorted.end(), a[i]);
        rank[i] = int(it - sorted.begin()) + 1;
    }
    return rank;
}

vector<int> discretize(const vector<ll> &a) {
    vector<ll> sorted;
    return discretize(a, sorted);
}
