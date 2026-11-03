/**
 * @brief 后缀自动机（SAM）
 * @complexity 固定字母表建树 O(n)，单次查串 O(m)
 * @index 状态 0 为初态
 * @idea 每追加一字符，沿后缀链接补转移。若旧状态最长长度不合适，就克隆状态来分开 endpos 等价类。
 * @usage SuffixAutomaton sam;
 *   for (char c : text) sam.append(c);
 *   long long kinds = sam.distinct();
 *   sam.prepare_occurrences();
 *   long long hits = sam.occurrences(pattern);
 * @pitfall 字符为小写字母。先追加完再 prepare_occurrences()，继续追加后需重算出现次数。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SuffixAutomaton {
    struct State {
        array<int, 26> next{};
        int length = 0, link = -1;
        int ends = 0;
    };
    vector<State> states{State{}};
    vector<ll> count;
    int last = 0;

    void append(char ch) {
        int c = ch - 'a', u = (int)states.size();
        states.emplace_back();
        states[u].length = states[last].length + 1;
        states[u].ends = 1;
        int p = last;
        for (; p != -1 && !states[p].next[c]; p = states[p].link)
            states[p].next[c] = u;
        if (p == -1) {
            states[u].link = 0;
        } else {
            int q = states[p].next[c];
            if (states[q].length == states[p].length + 1) {
                states[u].link = q;
            } else {
                int clone = (int)states.size();
                states.push_back(states[q]);
                states[clone].length = states[p].length + 1;
                states[clone].ends = 0;
                for (; p != -1 && states[p].next[c] == q; p = states[p].link)
                    states[p].next[c] = clone;
                states[q].link = states[u].link = clone;
            }
        }
        last = u;
        count.clear();
    }

    ll distinct() const {
        ll answer = 0;
        for (int u = 1; u < (int)states.size(); u++)
            answer += states[u].length - states[states[u].link].length;
        return answer;
    }

    void prepare_occurrences() {
        vector<int> frequency(states[last].length + 1), order(states.size());
        for (const auto &a : states)
            frequency[a.length]++;
        for (int i = 1; i < (int)frequency.size(); i++)
            frequency[i] += frequency[i - 1];
        for (int u = (int)states.size() - 1; u >= 0; u--)
            order[--frequency[states[u].length]] = u;

        count.resize(states.size());
        for (int u = 0; u < (int)states.size(); u++)
            count[u] = states[u].ends;
        for (int i = (int)order.size() - 1; i > 0; i--) {
            int u = order[i];
            count[states[u].link] += count[u];
        }
    }

    ll occurrences(string_view pattern) const {
        assert(count.size() == states.size());
        int u = 0;
        for (char ch : pattern) {
            u = states[u].next[ch - 'a'];
            if (!u)
                return 0;
        }
        return count[u] + pattern.empty();
    }
};
