/**
 * @brief FFT 整数卷积
 * @complexity O(n log n)
 * @index 系数低次在前
 * @idea 用复数单位根做正变换，逐点乘后用反向单位根还原，取实部四舍五入。
 * @usage fft_multiply(a,b) 返回整数卷积。
 * @pitfall 使用 long double，系数或长度过大时可能舍入出错。精确模卷积用 NTT，结果须放进 long long。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

inline void fft_transform(vector<complex<ld>> &a, bool inverse) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len *= 2) {
        ld angle = 2 * numbers::pi_v<ld> / len;
        auto root = polar(1.L, inverse ? -angle : angle);
        for (int i = 0; i < n; i += len) {
            complex<ld> w = 1;
            for (int j = 0; j < len / 2; j++) {
                auto u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= root;
            }
        }
    }
    if (inverse)
        for (auto &x : a)
            x /= n;
}

inline vector<ll> fft_multiply(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty())
        return {};
    int size = (int)(a.size() + b.size() - 1);
    int n = (int)bit_ceil((unsigned)size);
    vector<complex<ld>> x(n), y(n);
    for (int i = 0; i < (int)a.size(); i++)
        x[i] = (ld)a[i];
    for (int i = 0; i < (int)b.size(); i++)
        y[i] = (ld)b[i];
    fft_transform(x, false);
    fft_transform(y, false);
    for (int i = 0; i < n; i++)
        x[i] *= y[i];
    fft_transform(x, true);
    vector<ll> answer(size);
    for (int i = 0; i < size; i++)
        answer[i] = llround(x[i].real());
    return answer;
}
