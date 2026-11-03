/**
 * @brief Stern–Brocot 树路径
 * @complexity O(log max(p, q))
 * @index 从根 1/1 出发
 * @idea p < q 时往左，对应 p/q 变成 p/(q-p)。往右同理。一次用除法走完一段同向边。
 * @usage stern_brocot_path(p, q) 返回到 p/q 的压缩路径，每项为 {L 或 R, 连走次数}。
 * @pitfall p、q 必须为正，可以不互质。1/1 的路径为空。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline vector<pair<char, ull>> stern_brocot_path(ull p, ull q) {
    vector<pair<char, ull>> path;
    while (p != q) {
        if (p < q) {
            ull count = (q - 1) / p;
            q -= count * p;
            path.emplace_back('L', count);
        } else {
            ull count = (p - 1) / q;
            p -= count * q;
            path.emplace_back('R', count);
        }
    }
    return path;
}
