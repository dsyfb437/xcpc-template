/**
 * @brief 后缀数组（倍增）和 LCP
 * @complexity 建 SA O(n log n)，LCP O(n)
 * @index 后缀起点和排名 0-based
 * @idea 在末尾加最小哨兵。每轮按两个长度减半的排名做计数排序，再重新编号。LCP 用 Kasai 扫描。
 * @usage auto sa = suffix_array(s);
 *   auto lcp = lcp_array(s, sa);
 *   lcp[i] 是 sa[i-1] 和 sa[i] 的公共前缀长度，lcp[0] 为 0。
 * @pitfall 按无符号字节排序，sa 不含空后缀，lcp[0] 为 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> suffix_array(string_view s) {
    int n = (int)s.size() + 1;
    vector<int> sa(n), rank(n), next(n), shifted(n), count(max(257, n));
    auto symbol = [&](int i) {
        return i == n - 1 ? 0 : (int)(unsigned char)s[i] + 1;
    };
    for (int i = 0; i < n; i++)
        count[symbol(i)]++;
    for (int i = 1; i < 257; i++)
        count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--)
        sa[--count[symbol(i)]] = i;

    int classes = 1;
    for (int i = 1; i < n; i++) {
        classes += symbol(sa[i]) != symbol(sa[i - 1]);
        rank[sa[i]] = classes - 1;
    }
    for (int len = 1; len < n && classes < n; len *= 2) {
        fill(count.begin(), count.begin() + classes, 0);
        for (int i = 0; i < n; i++) {
            int u = sa[i] - len;
            if (u < 0)
                u += n;
            shifted[i] = u;
            count[rank[u]]++;
        }
        for (int i = 1; i < classes; i++)
            count[i] += count[i - 1];
        for (int i = n - 1; i >= 0; i--) {
            int u = shifted[i];
            sa[--count[rank[u]]] = u;
        }
        next[sa[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            int a = sa[i - 1], b = sa[i];
            classes += pair{rank[a], rank[(a + len) % n]} !=
                       pair{rank[b], rank[(b + len) % n]};
            next[b] = classes - 1;
        }
        rank.swap(next);
    }
    sa.erase(sa.begin());
    return sa;
}

inline vector<int> lcp_array(string_view s, const vector<int> &sa) {
    int n = (int)s.size();
    vector<int> rank(n), lcp(n);
    for (int i = 0; i < n; i++)
        rank[sa[i]] = i;
    for (int i = 0, same = 0; i < n; i++) {
        int k = rank[i];
        if (!k) {
            same = 0;
            continue;
        }
        int j = sa[k - 1];
        while (i + same < n && j + same < n && s[i + same] == s[j + same])
            same++;
        lcp[k] = same;
        if (same)
            same--;
    }
    return lcp;
}
