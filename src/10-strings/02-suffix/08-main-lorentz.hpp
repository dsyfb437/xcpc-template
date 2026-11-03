/**
 * @brief Main–Lorentz 相邻重复枚举
 * @complexity O(n log n)，另计展开结果
 * @index 起点 0-based，压缩范围含两端
 * @idea 分治枚举跨中点的平方串。按两份的分界点在左还是在右分组，用四个 Z 表求可向两侧匹配的长度。
 * @usage auto groups = tandem_repetitions(s);
 *   每组 {first, last, period} 表示：first 到 last 的每个起点，
 *   都有两个紧挨着的、长度为 period 的相同子串。
 * @pitfall 枚举的是两份相邻相同子串，period 不一定最小。压缩表示中每个 (start,period) 只出现一次。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct TandemGroup {
    int first, last, period;
};

inline vector<TandemGroup> tandem_repetitions(string_view s) {
    vector<int> text;
    for (unsigned char c : s)
        text.push_back(c);
    auto z_function = [](const vector<int> &a) {
        int n = (int)a.size();
        vector<int> z(n);
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (i < r)
                z[i] = min(r - i, z[i - l]);
            while (i + z[i] < n && a[z[i]] == a[i + z[i]])
                z[i]++;
            if (i + z[i] > r) {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    };
    auto cross_z = [&](vector<int> prefix, const vector<int> &suffix) {
        prefix.push_back(256);
        prefix.insert(prefix.end(), suffix.begin(), suffix.end());
        return z_function(prefix);
    };

    vector<TandemGroup> answer;
    auto solve = [&](auto &&self, int l, int r) -> void {
        if (r - l < 2)
            return;
        int mid = (l + r) / 2, a = mid - l, b = r - mid;
        self(self, l, mid);
        self(self, mid, r);

        vector<int> left(text.begin() + l, text.begin() + mid);
        vector<int> right(text.begin() + mid, text.begin() + r);
        vector<int> rev_left(left.rbegin(), left.rend());
        vector<int> rev_right(right.rbegin(), right.rend());
        auto zl = z_function(rev_left), zr = z_function(right);
        auto forward = cross_z(right, left);
        auto backward = cross_z(rev_left, rev_right);

        for (int p = 1; p <= a; p++) {
            int anchor = mid - p;
            int before = p < a ? zl[p] : 0;
            int after = forward[b + 1 + a - p];
            int low = max(mid - 2 * p + 1, anchor - before);
            int high = min(anchor, anchor + after - p);
            if (low <= high)
                answer.push_back({low, high, p});
        }
        for (int p = 1; p < b; p++) {
            int before = backward[a + 1 + b - p];
            int after = zr[p];
            int low = max(mid - p + 1, mid - before);
            int high = min(mid - 1, mid + after - p);
            if (low <= high)
                answer.push_back({low, high, p});
        }
    };
    solve(solve, 0, (int)text.size());
    return answer;
}
