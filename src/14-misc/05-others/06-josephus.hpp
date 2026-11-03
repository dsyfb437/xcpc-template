/**
 * @brief 约瑟夫问题（最后留下谁）
 * @complexity O(n) 时间，O(1) 空间
 * @index 编号从 0 开始
 * @idea 从只剩一人倒推。圈里有 size 人时，把小圈的答案向后平移 step，再取模。
 * @usage auto survivor = josephus_survivor(n, step);
 *   第一次从 0 开始数，第 step 个出圈。
 * @pitfall n 和 step 都为正。要 1-based 的编号就给结果加 1。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

inline ull josephus_survivor(ull n, ull step) {
    assert(n > 0 && step > 0);
    ull answer = 0;
    for (ull size = 1; size < n;) {
        size++;
        ull shift = step % size;
        if (answer >= size - shift)
            answer -= size - shift;
        else
            answer += shift;
    }
    return answer;
}
