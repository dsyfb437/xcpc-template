/**
 * @brief Lyndon 分解（Duval）
 * @complexity O(n)
 * @index 返回半开区间 [l,r)
 * @idea 从左到右比较候选块和后续字符。遇到可重复的最小块时，按它的长度切出所有连续重复。
 * @usage lyndon_factorization(s) 返回按字典序非增的 Lyndon 因子区间。
 * @pitfall 按无符号字节比较，空串返回空表。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<pair<int, int>> lyndon_factorization(string_view s) {
    vector<pair<int, int>> factors;
    int n = (int)s.size();
    for (int start = 0; start < n;) {
        int j = start + 1, k = start;
        while (j < n && (unsigned char)s[k] <= (unsigned char)s[j]) {
            if (s[k] == s[j])
                k++;
            else
                k = start;
            j++;
        }
        int length = j - k;
        while (start <= k) {
            factors.emplace_back(start, start + length);
            start += length;
        }
    }
    return factors;
}
