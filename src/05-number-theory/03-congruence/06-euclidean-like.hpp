/**
 * @brief 类欧几里得（floor sum）
 * @complexity O(log max(a, m))
 * @index i 从 0 到 n - 1
 * @idea 先把 a 和 b 的整除部分直接加到答案。剩下的格点计数交换横纵轴，交换后的模数更小。
 * @usage floor_sum(n, m, a, b) 求 sum floor((a*i + b)/m)。
 * @pitfall n、a、b 非负，m 为正。需要 unsigned __int128，答案须放进该类型。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline unsigned __int128 floor_sum(ull n, ull m, ull a, ull b) {
    using Wide = unsigned __int128;
    Wide answer = 0;
    while (n) {
        answer += (Wide)n * (n - 1) / 2 * (a / m);
        answer += (Wide)n * (b / m);
        a %= m;
        b %= m;
        Wide top = (Wide)a * n + b;
        if (top < m)
            break;
        n = top / m;
        b = top % m;
        swap(a, m);
    }
    return answer;
}
