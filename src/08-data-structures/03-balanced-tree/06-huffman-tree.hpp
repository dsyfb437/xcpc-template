/**
 * @brief 霍夫曼树
 * @complexity O(n log n)
 * @index 编码长度与输入顺序一致
 * @idea 每次把权最小的两棵树合并。合并权累加就是带权路径长度，从最终根遍历得到各符号的编码长度。
 * @usage auto [cost,length] = huffman_lengths(weight);
 * @pitfall 权值非负，可为 0。单个符号的编码长度按 0 计，代价用 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline pair<__int128, vector<int>> huffman_lengths(const vector<ll> &w) {
    int n = (int)w.size();
    if (!n)
        return {0, {}};
    using Item = pair<__int128, int>;
    priority_queue<Item, vector<Item>, greater<Item>> heap;
    for (int i = 0; i < n; i++)
        heap.emplace(w[i], i);

    vector<pair<int, int>> children;
    __int128 cost = 0;
    while (heap.size() > 1) {
        auto [a, u] = heap.top();
        heap.pop();
        auto [b, v] = heap.top();
        heap.pop();
        cost += a + b;
        heap.emplace(a + b, n + (int)children.size());
        children.emplace_back(u, v);
    }
    vector<int> length(n);
    vector<pair<int, int>> stack{{heap.top().second, 0}};
    while (!stack.empty()) {
        auto [u, depth] = stack.back();
        stack.pop_back();
        if (u < n) {
            length[u] = depth;
        } else {
            auto [a, b] = children[u - n];
            stack.emplace_back(a, depth + 1);
            stack.emplace_back(b, depth + 1);
        }
    }
    return {cost, move(length)};
}
