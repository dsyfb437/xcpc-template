/**
 * @brief 回溯法（枚举 N 皇后）
 * @complexity 搜索节点数最坏 O(n!)，每个节点试 n 列。额外空间 O(n)
 * @index 列号和行号都从 0 开始
 * @idea 每层只放一行。列和两条对角线都空着才能放，递归返回后撤销三个标记。
 * @usage enumerate_n_queens(n, [&](const vector<int> &col) {
 *       // col[r] 是第 r 行皇后的列号。要保存解就复制一份。
 *   });
 * @pitfall 两条对角线分别用 r+c 和 r-c+n 编号。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Visit> void enumerate_n_queens(int n, Visit visit) {
    vector<int> col(n);
    vector<bool> used_col(n), used_sum(2 * n + 1), used_diff(2 * n + 1);
    auto dfs = [&](auto &&self, int r) -> void {
        if (r == n) {
            visit(as_const(col));
            return;
        }
        for (int c = 0; c < n; c++) {
            int sum = r + c, diff = r - c + n;
            if (used_col[c] || used_sum[sum] || used_diff[diff])
                continue;
            col[r] = c;
            used_col[c] = used_sum[sum] = used_diff[diff] = true;
            self(self, r + 1);
            used_col[c] = used_sum[sum] = used_diff[diff] = false;
        }
    };
    dfs(dfs, 0);
}
