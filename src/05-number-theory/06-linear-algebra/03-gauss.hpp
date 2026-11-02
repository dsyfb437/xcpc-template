/**
 * @brief 高斯消元（实数）
 * @complexity O(n^3)
 * @index vector 0-based
 *
 * 解 n 元一次方程组 A x = b；无解或无穷多解返回空 vector。
 * 例：
 *   vector<vector<ld>> A = {{2, 1}, {1, 3}};
 *   vector<ld> b = {5, 10};
 *   vector<ld> x = gauss(A, b);
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

inline vector<ld> gauss(vector<vector<ld>> a, vector<ld> b) {
    int n = a.size();
    const ld eps = 1e-12;
    for (int col = 0; col < n; col++) {
        int piv = col;
        for (int r = col + 1; r < n; r++)
            if (fabs(a[r][col]) > fabs(a[piv][col]))
                piv = r;
        if (fabs(a[piv][col]) < eps)
            return {};
        swap(a[col], a[piv]);
        swap(b[col], b[piv]);
        for (int r = 0; r < n; r++) {
            if (r == col)
                continue;
            ld t = a[r][col] / a[col][col];
            for (int c = col; c < n; c++)
                a[r][c] -= t * a[col][c];
            b[r] -= t * b[col];
        }
    }
    vector<ld> x(n);
    for (int i = 0; i < n; i++)
        x[i] = b[i] / a[i][i];
    return x;
}
