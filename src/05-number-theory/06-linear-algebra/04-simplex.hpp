/**
 * @brief 单纯形法（两阶段）
 * @complexity 最坏指数级
 * @index 变量和约束 0-based
 * @idea 先加人工变量找可行基，再沿能改善目标的方向换基。按变量编号打破平局。
 * @usage linear_program(A, b, c) 最大化 c·x，约束 A x <= b 且 x >= 0。返回状态、最优值和解。
 * @pitfall 浮点模板，适合小规模且数值条件较好的问题。状态为 optimal、infeasible 或 unbounded。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

struct LinearProgramResult {
    enum Status { optimal, infeasible, unbounded };
    Status status;
    ld value;
    vector<ld> x;
};

inline LinearProgramResult linear_program(const vector<vector<ld>> &A,
                                          const vector<ld> &b,
                                          const vector<ld> &c) {
    constexpr ld eps = 1e-10L;
    int m = (int)b.size(), n = (int)c.size();
    vector<int> basic(m), nonbasic(n + 1);
    iota(basic.begin(), basic.end(), n);
    iota(nonbasic.begin(), nonbasic.end(), 0);
    nonbasic[n] = -1;
    vector d(m + 2, vector<ld>(n + 2));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            d[i][j] = A[i][j];
        d[i][n] = -1;
        d[i][n + 1] = b[i];
    }
    for (int j = 0; j < n; j++)
        d[m][j] = -c[j];
    d[m + 1][n] = 1;

    auto pivot = [&](int r, int s) {
        ld inv = 1 / d[r][s];
        for (int i = 0; i < m + 2; i++)
            if (i != r)
                for (int j = 0; j < n + 2; j++)
                    if (j != s)
                        d[i][j] -= d[r][j] * d[i][s] * inv;
        for (int j = 0; j < n + 2; j++)
            if (j != s)
                d[r][j] *= inv;
        for (int i = 0; i < m + 2; i++)
            if (i != r)
                d[i][s] *= -inv;
        d[r][s] = inv;
        swap(basic[r], nonbasic[s]);
    };
    auto optimize = [&](int row) {
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; j++) {
                if (nonbasic[j] == -1 || d[row][j] >= -eps)
                    continue;
                if (s == -1 || nonbasic[j] < nonbasic[s])
                    s = j;
            }
            if (s == -1)
                return true;
            int r = -1;
            ld best = 0;
            for (int i = 0; i < m; i++) {
                if (d[i][s] <= eps)
                    continue;
                ld ratio = d[i][n + 1] / d[i][s];
                if (r == -1 || ratio < best - eps ||
                    (abs(ratio - best) <= eps && basic[i] < basic[r])) {
                    r = i;
                    best = ratio;
                }
            }
            if (r == -1)
                return false;
            pivot(r, s);
        }
    };

    int r = -1;
    for (int i = 0; i < m; i++)
        if (r == -1 || d[i][n + 1] < d[r][n + 1])
            r = i;
    if (r != -1 && d[r][n + 1] < -eps) {
        pivot(r, n);
        if (!optimize(m + 1) || d[m + 1][n + 1] < -eps)
            return {LinearProgramResult::infeasible, 0, {}};
        for (int i = 0; i < m; i++)
            if (basic[i] == -1) {
                for (int j = 0; j <= n; j++)
                    if (abs(d[i][j]) > eps) {
                        pivot(i, j);
                        break;
                    }
                break;
            }
    }
    if (!optimize(m))
        return {LinearProgramResult::unbounded, 0, {}};
    vector<ld> x(n);
    for (int i = 0; i < m; i++)
        if (0 <= basic[i] && basic[i] < n)
            x[basic[i]] = d[i][n + 1];
    return {LinearProgramResult::optimal, d[m][n + 1], move(x)};
}
