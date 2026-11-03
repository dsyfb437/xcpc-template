/**
 * @brief 十五数码（最短移动序列）
 * @complexity IDA* 最坏指数级时间，栈空间 O(步数上限)
 * @index 棋盘按行展开，0 是空格，目标为 1…15,0
 * @idea 先判逆序数与空格行的奇偶。估价用曼哈顿距离，再加同行同列的顺序冲突。
 * @usage auto moves = solve_15_puzzle(board);
 *   board 是 array<int, 16>。结果 UDLR 表示空格的移动方向。
 * @pitfall 输入应为 0 到 15 的排列。无解或超过 max_depth 返回 nullopt，空字符串表示已经拼好。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline optional<string> solve_15_puzzle(array<int, 16> board, int max_depth = 80) {
    array<bool, 16> seen{};
    int blank = 0, inversions = 0;
    for (int i = 0; i < 16; i++) {
        int tile = board[i];
        if (tile < 0 || tile >= 16 || seen[tile])
            return nullopt;
        seen[tile] = true;
        if (tile == 0)
            blank = i;
        else {
            for (int j = 0; j < i; j++)
                if (board[j] > tile)
                    inversions++;
        }
    }
    if ((inversions + 4 - blank / 4) % 2 != 1)
        return nullopt;

    int dist[16][16]{}, estimate = 0;
    for (int tile = 1; tile < 16; tile++) {
        int goal = tile - 1;
        for (int pos = 0; pos < 16; pos++)
            dist[tile][pos] = abs(pos / 4 - goal / 4) + abs(pos % 4 - goal % 4);
    }
    for (int pos = 0; pos < 16; pos++)
        estimate += dist[board[pos]][pos];

    // 已在目标行（列）的棋子里，至多 LIS 长度个能留在这条线上。
    auto conflict = [&](int line, bool row) {
        int tails[4], kept = 0, total = 0;
        for (int i = 0; i < 4; i++) {
            int tile = board[row ? 4 * line + i : 4 * i + line];
            if (tile == 0)
                continue;
            int goal = tile - 1;
            if ((row ? goal / 4 : goal % 4) != line)
                continue;
            int along = row ? goal % 4 : goal / 4;
            int k = lower_bound(tails, tails + kept, along) - tails;
            tails[k] = along;
            if (k == kept)
                kept++;
            total++;
        }
        return 2 * (total - kept);
    };
    for (int i = 0; i < 4; i++)
        estimate += conflict(i, true) + conflict(i, false);

    constexpr int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    constexpr string_view direction = "UDLR";
    string path;
    int limit = estimate, next_limit;
    auto dfs = [&](auto &&self, int pos, int depth, int h, int prev) -> bool {
        int score = depth + h;
        if (score > limit) {
            next_limit = min(next_limit, score);
            return false;
        }
        if (h == 0)
            return true;

        for (int move = 0; move < 4; move++) {
            if (move == (prev ^ 1))
                continue;
            int r = pos / 4 + dr[move], c = pos % 4 + dc[move];
            if (r < 0 || r >= 4 || c < 0 || c >= 4)
                continue;
            int to = 4 * r + c, tile = board[to];
            bool vertical = dr[move] != 0;
            int a = vertical ? pos / 4 : pos % 4;
            int b = vertical ? r : c;
            // 竖移只改两行的相对次序，横移只改两列。
            int updated = h + dist[tile][pos] - dist[tile][to] -
                          conflict(a, vertical) - conflict(b, vertical);
            swap(board[pos], board[to]);
            updated += conflict(a, vertical) + conflict(b, vertical);
            path.push_back(direction[move]);
            if (self(self, to, depth + 1, updated, move))
                return true;
            path.pop_back();
            swap(board[pos], board[to]);
        }
        return false;
    };

    while (limit <= max_depth) {
        next_limit = numeric_limits<int>::max();
        if (dfs(dfs, blank, 0, estimate, -1))
            return path;
        if (next_limit == numeric_limits<int>::max())
            break;
        limit = next_limit;
    }
    return nullopt;
}
