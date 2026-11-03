/**
 * @brief 块状链表（插入、删除、取值）
 * @complexity 单次 O(n/B + B)
 * @index 位置从 0 起
 * @idea 链表中每个节点存一小段连续数组。块太大就拆开，删除导致块太小就和下一块合并。
 * @usage BlockList<T> a(B); a.insert(pos,x); a.erase(pos); a.at(pos)。
 * @pitfall B 至少为 1，通常取预计最大长度的平方根。插入位置可以是当前末尾，取值和删除必须在已有元素内。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T> struct BlockList {
    using Iterator = typename list<vector<T>>::iterator;
    list<vector<T>> blocks = list<vector<T>>(1);
    int width, count = 0;

    explicit BlockList(int B) : width(B) { assert(B >= 1); }
    int size() const { return count; }

    pair<Iterator, int> locate(int pos) {
        auto it = blocks.begin();
        while (next(it) != blocks.end() && pos >= (int)it->size()) {
            pos -= (int)it->size();
            ++it;
        }
        return {it, pos};
    }

    void split(Iterator it) {
        if ((int)it->size() <= 2 * width)
            return;
        auto middle = it->begin() + it->size() / 2;
        blocks.emplace(next(it), make_move_iterator(middle),
                       make_move_iterator(it->end()));
        it->erase(middle, it->end());
    }

    void insert(int pos, T value) {
        assert(0 <= pos && pos <= count);
        auto [it, offset] = locate(pos);
        it->insert(it->begin() + offset, move(value));
        count++;
        split(it);
    }

    void erase(int pos) {
        assert(0 <= pos && pos < count);
        auto [it, offset] = locate(pos);
        it->erase(it->begin() + offset);
        count--;
        auto after = next(it);
        if ((int)it->size() < width && after != blocks.end()) {
            it->insert(it->end(), make_move_iterator(after->begin()),
                       make_move_iterator(after->end()));
            blocks.erase(after);
            split(it);
        }
    }

    T &at(int pos) {
        assert(0 <= pos && pos < count);
        auto [it, offset] = locate(pos);
        return (*it)[offset];
    }
};
