/**
 * @brief 主元素（Boyer–Moore 投票）
 * @complexity O(n) 时间，O(1) 额外空间
 * @index -
 * @idea 两个不同的数两两抵消。若有超过半数的值，它一定是最后剩下的候选。
 * @usage auto majority = majority_element(a);
 *   没有出现超过半数的值时返回 nullopt。
 * @pitfall 最后必须再数一遍。恰好一半不算主元素。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T> optional<T> majority_element(const vector<T> &a) {
    size_t candidate = 0, balance = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (balance == 0)
            candidate = i;
        if (a[i] == a[candidate])
            balance++;
        else
            balance--;
    }
    if (balance == 0)
        return nullopt;
    size_t copies = count(a.begin(), a.end(), a[candidate]);
    if (copies <= a.size() / 2)
        return nullopt;
    return a[candidate];
}
