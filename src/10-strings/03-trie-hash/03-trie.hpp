/**
 * @brief Trie（字符串多重集）
 * @complexity 单次 O(L)
 * @index 节点 0 为根
 * @idea 沿字符边走，经过的节点加前缀计数，终点另记完整单词计数。删除时沿同一路径减去。
 * @usage StringTrie trie;
 *   trie.insert(word);
 *   int copies = trie.count(word);
 *   int with_prefix = trie.prefix_count(prefix);
 *   trie.erase(word);  // 删掉一份
 * @pitfall 字符为小写字母，支持空串和重复串。删除不存在的单词返回 false，节点不回收。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct StringTrie {
    struct Node {
        array<int, 26> next{};
        int prefix = 0, terminal = 0;
    };
    vector<Node> nodes{Node{}};

    void insert(string_view s) {
        int u = 0;
        nodes[u].prefix++;
        for (char ch : s) {
            int c = ch - 'a';
            if (!nodes[u].next[c]) {
                nodes[u].next[c] = (int)nodes.size();
                nodes.emplace_back();
            }
            u = nodes[u].next[c];
            nodes[u].prefix++;
        }
        nodes[u].terminal++;
    }

    int find(string_view s) const {
        int u = 0;
        for (char ch : s) {
            u = nodes[u].next[ch - 'a'];
            if (!u)
                return -1;
        }
        return u;
    }

    int count(string_view s) const {
        int u = find(s);
        return u == -1 ? 0 : nodes[u].terminal;
    }

    int prefix_count(string_view s) const {
        int u = find(s);
        return u == -1 ? 0 : nodes[u].prefix;
    }

    bool erase(string_view s) {
        if (!count(s))
            return false;
        int u = 0;
        nodes[u].prefix--;
        for (char ch : s) {
            u = nodes[u].next[ch - 'a'];
            nodes[u].prefix--;
        }
        nodes[u].terminal--;
        return true;
    }
};
