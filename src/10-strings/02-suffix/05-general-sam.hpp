/**
 * @brief 广义后缀自动机（Trie 建 SAM）
 * @complexity 固定字母表 O(S)，S 为总长度
 * @index 状态 0 为初态
 * @idea 先把所有字符串插进 Trie，再按 BFS 把边扩展进 SAM。已存在的转移若长度不对也要克隆。
 * @usage GeneralSAM sam(words);
 *   bool found = sam.contains(pattern);
 *   long long kinds = sam.distinct();  // 所有串的不同非空子串数
 * @pitfall 字符为小写字母，允许空串和重复串。不会把不同串的边界拼成子串。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct GeneralSAM {
    struct State {
        array<int, 26> next{};
        int length = 0, link = -1;
    };
    vector<State> states{State{}};

    explicit GeneralSAM(const vector<string> &words) {
        vector<array<int, 26>> trie(1);
        for (const auto &word : words) {
            int u = 0;
            for (char ch : word) {
                int c = ch - 'a';
                if (!trie[u][c]) {
                    trie[u][c] = (int)trie.size();
                    trie.emplace_back();
                }
                u = trie[u][c];
            }
        }
        vector<int> id(trie.size());
        queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < 26; c++)
                if (int v = trie[u][c]) {
                    id[v] = extend(id[u], c);
                    q.push(v);
                }
        }
    }

    int clone(int p, int q, int c) {
        int u = (int)states.size();
        states.push_back(states[q]);
        states[u].length = states[p].length + 1;
        for (; p != -1 && states[p].next[c] == q; p = states[p].link)
            states[p].next[c] = u;
        states[q].link = u;
        return u;
    }

    int extend(int p, int c) {
        if (int q = states[p].next[c]) {
            if (states[q].length == states[p].length + 1)
                return q;
            return clone(p, q, c);
        }

        int u = (int)states.size();
        states.emplace_back();
        states[u].length = states[p].length + 1;
        for (; p != -1 && !states[p].next[c]; p = states[p].link)
            states[p].next[c] = u;
        if (p == -1) {
            states[u].link = 0;
        } else {
            int q = states[p].next[c];
            if (states[q].length == states[p].length + 1)
                states[u].link = q;
            else
                states[u].link = clone(p, q, c);
        }
        return u;
    }

    bool contains(string_view pattern) const {
        int u = 0;
        for (char ch : pattern) {
            u = states[u].next[ch - 'a'];
            if (!u)
                return false;
        }
        return true;
    }

    ll distinct() const {
        ll result = 0;
        for (int u = 1; u < (int)states.size(); u++)
            result += states[u].length - states[states[u].link].length;
        return result;
    }
};
