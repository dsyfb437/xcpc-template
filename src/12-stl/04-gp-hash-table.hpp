/**
 * @brief gp_hash_table（开放寻址哈希表）
 * @complexity 增删查平均 O(1)
 * @index 键类型自行决定
 * @idea 用法类似 unordered_map。第三个模板参数可以传自定义哈希。
 * @usage GpHashMap<int, int> count;
 *   ++count[x];
 *   auto it = count.find(y);  // 只查时用 find，避免插入新键
 * @pitfall GNU libstdc++ 扩展。增删后重新取迭代器。默认策略下没有 unordered_map 那套 reserve 用法。
 */
#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace std;
using ll = long long;

template <class Key, class Value, class Hash = hash<Key>>
using GpHashMap = __gnu_pbds::gp_hash_table<Key, Value, Hash>;
