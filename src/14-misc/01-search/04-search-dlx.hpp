/**
 * @brief Dancing Links（精确覆盖）
 * @complexity 建表 O(列数 + 1 的个数)，搜索最坏指数级
 * @index 列号 0-based，返回行号
 * @idea 选剩余候选行最少的列。选中一行后，摘掉它覆盖的列和冲突行，回溯时按相反顺序接回去。
 * @usage DancingLinks dlx(m);
 *   dlx.add_row(id, columns);  // columns 写这一行的所有 1 所在列
 *   auto rows = dlx.solve();  // nullopt 表示无解
 * @pitfall 每列必须恰好覆盖一次。同一行内列号不能重复。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class DancingLinks {
    struct Node {
        int left, right, up, down, column, row;
    };
    vector<Node> nodes;
    vector<int> count;
    vector<int> chosen;

    void cover(int c) {
        nodes[nodes[c].left].right = nodes[c].right;
        nodes[nodes[c].right].left = nodes[c].left;
        for (int r = nodes[c].down; r != c; r = nodes[r].down) {
            for (int i = nodes[r].right; i != r; i = nodes[i].right) {
                nodes[nodes[i].up].down = nodes[i].down;
                nodes[nodes[i].down].up = nodes[i].up;
                count[nodes[i].column]--;
            }
        }
    }

    void uncover(int c) {
        for (int r = nodes[c].up; r != c; r = nodes[r].up) {
            for (int i = nodes[r].left; i != r; i = nodes[i].left) {
                nodes[nodes[i].up].down = i;
                nodes[nodes[i].down].up = i;
                count[nodes[i].column]++;
            }
        }
        nodes[nodes[c].left].right = c;
        nodes[nodes[c].right].left = c;
    }

    bool search() {
        int c = nodes[0].right;
        if (c == 0)
            return true;
        for (int i = nodes[c].right; i != 0; i = nodes[i].right)
            if (count[i] < count[c])
                c = i;
        if (count[c] == 0)
            return false;

        cover(c);
        for (int r = nodes[c].down; r != c; r = nodes[r].down) {
            chosen.push_back(nodes[r].row);
            for (int i = nodes[r].right; i != r; i = nodes[i].right)
                cover(nodes[i].column);
            bool solved = search();
            for (int i = nodes[r].left; i != r; i = nodes[i].left)
                uncover(nodes[i].column);
            if (solved) {
                uncover(c);
                return true;
            }
            chosen.pop_back();
        }
        uncover(c);
        return false;
    }

  public:
    explicit DancingLinks(int columns) : nodes(columns + 1), count(columns + 1) {
        for (int c = 0; c <= columns; c++) {
            int left = c == 0 ? columns : c - 1;
            int right = c == columns ? 0 : c + 1;
            nodes[c] = {left, right, c, c, c, -1};
        }
    }

    void add_row(int id, const vector<int> &columns) {
        int first = (int)nodes.size();
        for (int index : columns) {
            int c = index + 1;
            int i = (int)nodes.size();
            int up = nodes[c].up;
            if (i == first) {
                nodes.push_back({i, i, up, c, c, id});
            } else {
                int last = nodes[first].left;
                nodes.push_back({last, first, up, c, c, id});
                nodes[last].right = i;
                nodes[first].left = i;
            }
            nodes[up].down = i;
            nodes[c].up = i;
            count[c]++;
        }
    }

    optional<vector<int>> solve() {
        chosen.clear();
        if (!search())
            return nullopt;
        return chosen;
    }
};
