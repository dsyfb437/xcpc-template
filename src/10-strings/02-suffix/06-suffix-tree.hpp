/**
 * @brief 后缀树（由 SA 和 LCP 建树）
 * @complexity 预处理 O(n log n)，匹配 O(m)，输出 O(出现次数)
 * @index 根编号 0，后缀起点 0-based
 * @idea 先倍增建 SA，再用相邻 LCP 和栈补分叉点。每条边只保存原串上的起点和深度差。
 * @usage SuffixTree tree(text);
 *   bool found = tree.contains(pattern);
 *   auto positions = tree.occurrences(pattern);
 * @pitfall 按字节处理，树内加最小哨兵，结果未按文本位置排序。空模式匹配 0 到 n。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace suffix_tree_detail {
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
    return sa;
}

} // namespace suffix_tree_detail

struct SuffixTree {
    struct Node {
        int edge_start, depth, suffix;
        map<int, int> children;
    };
    string text;
    vector<Node> nodes{{0, 0, -1, {}}};

    int symbol(int i) const {
        return i == (int)text.size() ? 0 : (int)(unsigned char)text[i] + 1;
    }

    explicit SuffixTree(string s) : text(move(s)) {
        auto sa = suffix_tree_detail::suffix_array(text);
        int n = (int)sa.size();
        vector<int> rank(n), lcp(n);
        for (int i = 0; i < n; i++)
            rank[sa[i]] = i;
        for (int i = 0, len = 0; i < n; i++) {
            int k = rank[i];
            if (!k) {
                len = 0;
                continue;
            }
            int j = sa[k - 1];
            while (i + len < n && j + len < n && symbol(i + len) == symbol(j + len))
                len++;
            lcp[k] = len;
            if (len)
                len--;
        }

        vector<int> path{0};
        for (int i = 0; i < n; i++) {
            int last = -1, length = lcp[i];
            while (nodes[path.back()].depth > length) {
                last = path.back();
                path.pop_back();
            }
            int p = path.back();
            if (nodes[p].depth < length) {
                int mid = (int)nodes.size(), start = nodes[last].edge_start;
                nodes.push_back({start, length, -1, {}});
                nodes[p].children[symbol(start)] = mid;
                nodes[last].edge_start += length - nodes[p].depth;
                nodes[mid].children[symbol(nodes[last].edge_start)] = last;
                path.push_back(mid);
                p = mid;
            }
            int leaf = (int)nodes.size(), start = sa[i] + length;
            nodes.push_back({start, n - sa[i], sa[i], {}});
            nodes[p].children[symbol(start)] = leaf;
            path.push_back(leaf);
        }
    }

    int find(string_view pattern) const {
        int u = 0, pos = 0, m = (int)pattern.size();
        while (pos < m) {
            auto it = nodes[u].children.find((unsigned char)pattern[pos] + 1);
            if (it == nodes[u].children.end())
                return -1;
            int v = it->second, length = nodes[v].depth - nodes[u].depth;
            for (int i = 0; i < length && pos < m; i++, pos++) {
                int expected = (unsigned char)pattern[pos] + 1;
                if (symbol(nodes[v].edge_start + i) != expected)
                    return -1;
            }
            u = v;
        }
        return u;
    }

    bool contains(string_view pattern) const { return find(pattern) != -1; }

    vector<int> occurrences(string_view pattern) const {
        int u = find(pattern);
        if (u == -1)
            return {};
        vector<int> result, stack{u};
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (nodes[v].suffix != -1)
                result.push_back(nodes[v].suffix);
            for (auto [c, child] : nodes[v].children)
                stack.push_back(child);
        }
        return result;
    }
};
