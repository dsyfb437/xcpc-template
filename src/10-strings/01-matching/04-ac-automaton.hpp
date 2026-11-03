/**
 * @brief AC 自动机（各模式出现次数）
 * @complexity 建树 O(26S)，匹配 O(n + S + k)
 * @index add 返回模式编号
 * @idea Trie 的失配边指向最长可用后缀。文本扫一遍记到达次数，再按 BFS 逆序沿失配边汇总。
 * @usage AhoCorasick ac;
 *   int id = ac.add(pattern);
 *   ac.build();
 *   long long hits = ac.count(text)[id];
 * @pitfall 字符为小写字母，所有模式加完后只 build 一次。允许重叠、重复模式，空模式计 n+1 次。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct AhoCorasick {
    struct Node {
        array<int, 26> next{};
        int fail = 0;
    };
    vector<Node> nodes{Node{}};
    vector<int> terminal, order;

    int add(string_view pattern) {
        int u = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            if (!nodes[u].next[c]) {
                nodes[u].next[c] = (int)nodes.size();
                nodes.emplace_back();
            }
            u = nodes[u].next[c];
        }
        terminal.push_back(u);
        return (int)terminal.size() - 1;
    }

    void build() {
        queue<int> q;
        for (int v : nodes[0].next)
            if (v)
                q.push(v);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);
            int f = nodes[u].fail;
            for (int c = 0; c < 26; c++) {
                int &v = nodes[u].next[c];
                if (v) {
                    nodes[v].fail = nodes[f].next[c];
                    q.push(v);
                } else {
                    v = nodes[f].next[c];
                }
            }
        }
    }

    vector<ll> count(string_view text) const {
        vector<ll> visits(nodes.size());
        visits[0] = 1;
        int u = 0;
        for (char ch : text) {
            u = nodes[u].next[ch - 'a'];
            visits[u]++;
        }
        for (auto it = order.rbegin(); it != order.rend(); ++it)
            visits[nodes[*it].fail] += visits[*it];
        vector<ll> answer;
        for (int id : terminal)
            answer.push_back(visits[id]);
        return answer;
    }
};
