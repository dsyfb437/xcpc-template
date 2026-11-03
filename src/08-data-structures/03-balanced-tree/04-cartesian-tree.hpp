/**
 * @brief 笛卡尔树
 * @complexity O(n)
 * @index 点编号按输入从 0 起，空为 -1
 * @idea 从左到右用单调栈维护当前右链。被弹出的最后一个点是新点的左儿子，新点是栈顶的右儿子。
 * @usage cartesian_tree(a) 返回 {根, 左儿子表, 右儿子表}。
 * @pitfall 按值建小根堆，中序遍历是原数组顺序。相等时较早位置在上方。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline tuple<int, vector<int>, vector<int>> cartesian_tree(const vector<ll> &a) {
    int n = (int)a.size();
    vector<int> left(n, -1), right(n, -1), stack;
    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!stack.empty() && a[stack.back()] > a[i]) {
            last = stack.back();
            stack.pop_back();
        }
        left[i] = last;
        if (!stack.empty())
            right[stack.back()] = i;
        stack.push_back(i);
    }
    return {stack.empty() ? -1 : stack.front(), move(left), move(right)};
}
