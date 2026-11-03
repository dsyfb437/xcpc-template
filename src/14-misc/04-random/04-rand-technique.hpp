/**
 * @brief 随机化技巧（蓄水池抽样）
 * @complexity O(n) 时间，O(min(n,k)) 空间
 * @index 从输入流的各个位置等概率地抽 k 个
 * @idea 先留下前 k 项。读到第 i 项时，在 [0,i) 均匀抽一个下标，落在池子里就替换。
 * @usage auto sample = reservoir_sample(a.begin(), a.end(), k, rng);
 *   rng 可以是 mt19937_64，输入也可以是一遍读完的流。
 * @pitfall 抽的是位置，原数据有重复时结果也可能重复。输入不足 k 项就全部返回，顺序不保证。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <input_iterator It, sentinel_for<It> End, class RNG>
vector<iter_value_t<It>> reservoir_sample(It first, End last, size_t k, RNG &rng) {
    vector<iter_value_t<It>> sample;
    if (k == 0)
        return sample;
    size_t seen = 0;
    for (; first != last; ++first) {
        seen++;
        if (sample.size() < k) {
            sample.push_back(*first);
        } else {
            size_t pos = uniform_int_distribution<size_t>(0, seen - 1)(rng);
            if (pos < k)
                sample[pos] = *first;
        }
    }
    return sample;
}
