/**
 * @brief 最优原地后缀排序（SA-IS，借用输出的高位）
 * @complexity O(n) 时间，输出之外 O(1) 空间
 * @index 0-based，结果含末尾哨兵的下标
 * @idea 输出的低 32 位放后缀下标。高 32 位临时放类型、桶指针或缩减串。
 *   缩减串从输出右边存，左边继续排序。每层把上一层的位置记在空槽里。
 * @usage vector<int> text = {2, 1, 2, 1, 0};
 *   auto sa = suffix_array_inplace(text);  // {4, 3, 1, 2, 0}
 * @pitfall 输入已离散化到 [0,n)，且只有末尾为 0。输出每项 64 位，
 *   依赖这部分高位当工作区。直接处理 string 时用前面的倍增版更省事。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

namespace inplace_sa_detail {
struct Workspace {
    static constexpr ull low_mask = 0xffffffff;
    static constexpr ull type_bit = ull{1} << 63;
    static constexpr ull data_mask = (type_bit - 1) & ~low_mask;

    span<const int> text;
    vector<ull> sa;
    int n;
    // source < 0 读原串，否则读 sa[source...] 的高位。
    int source = -1;

    int suffix(int i) const { return int(unsigned(sa[i])); }
    int data(int i) const { return int((sa[i] & data_mask) >> 32); }
    bool is_s(int i) const { return sa[i] & type_bit; }
    bool is_lms(int i) const { return i > 0 && is_s(i) && !is_s(i - 1); }

    void set_suffix(int i, int pos) { sa[i] = (sa[i] & ~low_mask) | unsigned(pos); }

    void set_data(int i, int value) {
        sa[i] = (sa[i] & ~data_mask) | (ull(value) << 32);
    }

    int symbol(int i) const { return source < 0 ? text[i] : data(source + i); }

    void classify() {
        sa[n - 1] |= type_bit;
        for (int i = n - 2; i >= 0; i--) {
            int a = symbol(i), b = symbol(i + 1);
            bool s = a < b || (a == b && is_s(i + 1));
            sa[i] &= ~type_bit;
            if (s)
                sa[i] |= type_bit;
        }
    }

    void buckets(bool tails) {
        for (int i = 0; i < n; i++)
            set_data(i, 0);
        for (int i = 0; i < n; i++) {
            int c = symbol(i);
            set_data(c, data(c) + 1);
        }
        int total = 0;
        for (int c = 0; c < n; c++) {
            int count = data(c);
            set_data(c, tails ? total + count : total);
            total += count;
        }
    }

    void put_at_tail(int pos) {
        int c = symbol(pos), slot = data(c) - 1;
        set_data(c, slot);
        set_suffix(slot, pos);
    }

    void induce() {
        buckets(false);
        for (int i = 0; i < n; i++) {
            int p = suffix(i) - 1;
            if (p < 0 || is_s(p))
                continue;
            int c = symbol(p), slot = data(c);
            set_data(c, slot + 1);
            set_suffix(slot, p);
        }
        buckets(true);
        for (int i = n - 1; i >= 0; i--) {
            int p = suffix(i) - 1;
            if (p >= 0 && is_s(p))
                put_at_tail(p);
        }
    }

    bool same_lms(int a, int b) const {
        for (int offset = 0;; offset++) {
            int i = a + offset, j = b + offset;
            if (symbol(i) != symbol(j) || is_s(i) != is_s(j))
                return false;
            if (offset && (is_lms(i) || is_lms(j)))
                return is_lms(i) && is_lms(j);
        }
    }

    void reduce() {
        classify();
        buckets(true);
        for (int i = 0; i < n; i++)
            set_suffix(i, -1);
        for (int i = n - 1; i > 0; i--)
            if (is_lms(i))
                put_at_tail(i);
        induce();

        int previous = -1, name = -1;
        for (int i = 0; i < n; i++) {
            int p = suffix(i);
            if (!is_lms(p))
                continue;
            if (previous < 0 || !same_lms(previous, p))
                name++;
            set_data(p, name);
            previous = p;
        }

        // 右移到尾部，写入的位置不会在尚未读取的名字左边。
        int start = n;
        for (int i = n - 1; i > 0; i--)
            if (is_lms(i))
                set_data(--start, data(i));
        set_suffix(n - 1, source);
        source = start;
        n -= start;
    }

    void expand() {
        int count = n, start = source;
        n += start;
        source = suffix(n - 1);
        classify();

        int slot = n;
        for (int i = n - 1; i > 0; i--)
            if (is_lms(i))
                set_data(--slot, i);
        for (int i = 0; i < count; i++)
            set_suffix(i, data(start + suffix(i)));

        buckets(true);
        for (int i = count; i < n; i++)
            set_suffix(i, -1);
        // 从后往前安放，桶里的新位置总在原列表位置的右边或原位。
        for (int i = count - 1; i >= 0; i--) {
            int p = suffix(i);
            set_suffix(i, -1);
            put_at_tail(p);
        }
        induce();
    }
};
} // namespace inplace_sa_detail

inline vector<ull> suffix_array_inplace(span<const int> text) {
    if (text.empty())
        return {};
    assert(text.size() <= size_t(numeric_limits<int>::max()));
    int n = int(text.size());
    assert(text.back() == 0);
    for (int i = 0; i + 1 < n; i++)
        assert(0 < text[i] && text[i] < n);

    inplace_sa_detail::Workspace work{text, vector<ull>(n), n};
    while (work.n > 1)
        work.reduce();
    work.set_suffix(0, 0);
    while (work.source >= 0)
        work.expand();
    for (auto &entry : work.sa)
        entry &= work.low_mask;
    return move(work.sa);
}
