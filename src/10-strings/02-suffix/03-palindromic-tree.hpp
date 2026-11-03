/**
 * @brief 回文自动机（PAM）
 * @complexity 固定字母表建树 O(n)，汇总 O(n)
 * @index 节点 0、1 为奇根和偶根
 * @idea 追加字符时沿 fail 找可扩展的最长回文后缀。每个非根节点代表一个不同回文，出现次数沿 fail 汇总。
 * @usage PalindromicTree pam;
 *   for (char c : text) pam.append(c);
 *   auto count = pam.occurrences();  // count[v] 是节点 v 的出现次数
 * @pitfall 字符为小写字母。nodes[u].end 是首次出现的右端点，length 是长度，两个根不计入答案。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct PalindromicTree {
    struct Node {
        array<int, 26> next{};
        int length = 0, fail = 0, end = -1, suffixes = 0;
    };
    vector<Node> nodes{Node{}, Node{}};
    string text;
    int last = 1;

    PalindromicTree() { nodes[0].length = -1; }

    int extendable(int u, int pos) const {
        while (true) {
            int left = pos - 1 - nodes[u].length;
            if (left >= 0 && text[left] == text[pos])
                return u;
            u = nodes[u].fail;
        }
    }

    int append(char ch) {
        int pos = (int)text.size(), c = ch - 'a';
        text.push_back(ch);
        int p = extendable(last, pos);
        if (!nodes[p].next[c]) {
            Node node;
            node.length = nodes[p].length + 2;
            node.end = pos;
            if (node.length == 1)
                node.fail = 1;
            else
                node.fail = nodes[extendable(nodes[p].fail, pos)].next[c];
            nodes[p].next[c] = (int)nodes.size();
            nodes.push_back(node);
        }
        last = nodes[p].next[c];
        nodes[last].suffixes++;
        return last;
    }

    vector<ll> occurrences() const {
        vector<ll> result(nodes.size());
        for (int u = (int)nodes.size() - 1; u >= 2; u--) {
            result[u] += nodes[u].suffixes;
            if (nodes[u].fail >= 2)
                result[nodes[u].fail] += result[u];
        }
        return result;
    }
};
