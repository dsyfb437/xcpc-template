/**
 * @brief pb_ds 平衡树（第 k 小、查排名）
 * @complexity 插入、删除、排名、第 k 小均 O(log n)
 * @index 第 k 小从 0 开始
 * @idea 用法跟 set 一样，另外有 order_of_key 和 find_by_order。
 * @usage OrderedSet<int> s;
 *   s.insert(x);
 *   int smaller = s.order_of_key(x);
 *   auto it = s.find_by_order(k);  // 越界返回 s.end()
 * @pitfall GNU libstdc++ 扩展。要存重复值，用 {值, 唯一编号} 作键。
 */
#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using ll = long long;

template <class Key>
using OrderedSet =
    __gnu_pbds::tree<Key, __gnu_pbds::null_type, less<Key>, __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;
