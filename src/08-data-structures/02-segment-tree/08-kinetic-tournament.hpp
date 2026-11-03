/**
 * @brief 动态锦标赛树（随时间变化的最大直线值）
 * @complexity 改线 O(log n)，取最大 O(1)，推进时间按失效节点计
 * @index 直线编号 0-based，时间为整数
 * @idea 每个节点存当前赢家和下次可能换赢家的时刻。时间前进后，只递归更新已失效的子树。
 * @usage KineticTournament t(lines,time); t.advance(time); t.replace(i,line); t.maximum() 返回 {值,编号}。
 * @pitfall 直线集非空，时间只增不减。用 __int128 保存值和交点。大量证书同时失效时，一次推进可能访问整树。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct KineticTournament {
    struct Line {
        ll slope, intercept;
        __int128 at(ll x) const { return (__int128)slope * x + intercept; }
    };
    static constexpr __int128 never = (__int128)1 << 126;
    vector<Line> lines;
    vector<int> winner;
    vector<__int128> expires;
    ll time;

    KineticTournament(vector<Line> a, ll start)
        : lines(move(a)), winner(4 * lines.size()), expires(winner.size(), never),
          time(start) {
        assert(!lines.empty());
        auto build = [&](auto &&self, int u, int l, int r) -> void {
            if (r - l == 1) {
                winner[u] = l;
                return;
            }
            int m = (l + r) / 2;
            self(self, 2 * u, l, m);
            self(self, 2 * u + 1, m, r);
            pull(u);
        };
        build(build, 1, 0, (int)lines.size());
    }

    void pull(int u) {
        int a = winner[2 * u], b = winner[2 * u + 1];
        auto va = lines[a].at(time), vb = lines[b].at(time);
        if (va < vb || (va == vb && lines[a].slope < lines[b].slope))
            swap(a, b);
        winner[u] = a;
        expires[u] = min(expires[2 * u], expires[2 * u + 1]);
        __int128 rate = (__int128)lines[b].slope - lines[a].slope;
        if (rate > 0) {
            __int128 gap = (__int128)lines[a].intercept - lines[b].intercept;
            __int128 change = gap / rate + (gap % rate > 0);
            expires[u] = min(expires[u], change);
        }
    }

    void advance(ll next_time) {
        assert(time <= next_time);
        time = next_time;
        auto refresh = [&](auto &&self, int u) -> void {
            if (expires[u] > time)
                return;
            self(self, 2 * u);
            self(self, 2 * u + 1);
            pull(u);
        };
        refresh(refresh, 1);
    }

    void replace(int i, Line line) {
        lines[i] = line;
        auto dfs = [&](auto &&self, int u, int l, int r) -> void {
            if (r - l == 1)
                return;
            int m = (l + r) / 2;
            if (i < m)
                self(self, 2 * u, l, m);
            else
                self(self, 2 * u + 1, m, r);
            pull(u);
        };
        dfs(dfs, 1, 0, (int)lines.size());
    }

    pair<__int128, int> maximum() const {
        int i = winner[1];
        return {lines[i].at(time), i};
    }
};
