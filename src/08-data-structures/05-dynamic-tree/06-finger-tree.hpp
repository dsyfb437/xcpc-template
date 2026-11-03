/**
 * @brief 手指树（可持久化序列）
 * @complexity 沿单一版本链的两端操作均摊 O(1)，拼接、切分和取下标 O(log n)
 * @index 下标与切分位置 0-based
 * @idea 两端各留 1 到 4 项。溢出时把三个同层项打包进中间树，取空时从中间拆回一组。用元素总数定位切分点。
 * @usage FingerTree<T> t; t=t.push_back(x); t=t.pop_front(); t.at(i); t.split(k); t.concat(other)。
 * @pitfall 操作返回新版本，原树仍可用。split 左段长度为 k，pop 要求非空。用共享指针保存不变节点。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T> class FingerTree {
    struct Item;
    using ItemPtr = shared_ptr<const Item>;
    using Digit = vector<ItemPtr>;
    struct Item {
        size_t size;
        optional<T> value;
        Digit children;
    };
    struct Tree;
    using TreePtr = shared_ptr<const Tree>;
    struct Tree {
        size_t size;
        ItemPtr single;
        Digit prefix, suffix;
        TreePtr middle;
    };

    TreePtr root;
    explicit FingerTree(TreePtr root) : root(move(root)) {}

    static size_t size(const TreePtr &t) { return t ? t->size : 0; }

    static size_t size(const Digit &digit) {
        size_t count = 0;
        for (const auto &x : digit)
            count += x->size;
        return count;
    }

    static TreePtr single(ItemPtr x) {
        return make_shared<Tree>(Tree{x->size, move(x), {}, {}, {}});
    }

    static TreePtr deep(Digit left, TreePtr middle, Digit right) {
        size_t count = size(left) + size(middle) + size(right);
        return make_shared<Tree>(
            Tree{count, {}, move(left), move(right), move(middle)});
    }

    static ItemPtr group(Digit children) {
        size_t count = size(children);
        return make_shared<Item>(Item{count, nullopt, move(children)});
    }

    static TreePtr prepend(const TreePtr &t, ItemPtr x) {
        if (!t)
            return single(move(x));
        if (t->single)
            return deep({move(x)}, {}, {t->single});
        auto left = t->prefix;
        if (left.size() == 4)
            return deep({move(x), left[0]},
                        prepend(t->middle, group({left[1], left[2], left[3]})),
                        t->suffix);
        left.insert(left.begin(), move(x));
        return deep(move(left), t->middle, t->suffix);
    }

    static TreePtr append(const TreePtr &t, ItemPtr x) {
        if (!t)
            return single(move(x));
        if (t->single)
            return deep({t->single}, {}, {move(x)});
        auto right = t->suffix;
        if (right.size() == 4)
            return deep(t->prefix,
                        append(t->middle, group({right[0], right[1], right[2]})),
                        {right[3], move(x)});
        right.push_back(move(x));
        return deep(t->prefix, t->middle, move(right));
    }

    static TreePtr from_digit(const Digit &digit) {
        TreePtr result;
        for (const auto &x : digit)
            result = append(result, x);
        return result;
    }

    static TreePtr with_left(Digit left, TreePtr middle, Digit right) {
        if (!left.empty())
            return deep(move(left), move(middle), move(right));
        if (!middle)
            return from_digit(right);
        auto [x, rest] = take_first(middle);
        return deep(x->children, move(rest), move(right));
    }

    static TreePtr with_right(Digit left, TreePtr middle, Digit right) {
        if (!right.empty())
            return deep(move(left), move(middle), move(right));
        if (!middle)
            return from_digit(left);
        auto [x, rest] = take_last(middle);
        return deep(move(left), move(rest), x->children);
    }

    static pair<ItemPtr, TreePtr> take_first(const TreePtr &t) {
        if (t->single)
            return {t->single, {}};
        auto left = t->prefix;
        auto item = left.front();
        left.erase(left.begin());
        return {item, with_left(move(left), t->middle, t->suffix)};
    }

    static pair<ItemPtr, TreePtr> take_last(const TreePtr &t) {
        if (t->single)
            return {t->single, {}};
        auto right = t->suffix;
        auto item = right.back();
        right.pop_back();
        return {item, with_right(t->prefix, t->middle, move(right))};
    }

    static TreePtr join(const TreePtr &a, const Digit &between, const TreePtr &b) {
        if (!a || a->single) {
            TreePtr t = b;
            for (auto it = between.rbegin(); it != between.rend(); ++it)
                t = prepend(t, *it);
            return a ? prepend(t, a->single) : t;
        }
        if (!b || b->single) {
            TreePtr t = a;
            for (const auto &x : between)
                t = append(t, x);
            return b ? append(t, b->single) : t;
        }

        Digit bridge = a->suffix, packed;
        bridge.insert(bridge.end(), between.begin(), between.end());
        bridge.insert(bridge.end(), b->prefix.begin(), b->prefix.end());
        for (size_t i = 0; i < bridge.size();) {
            size_t left = bridge.size() - i;
            size_t count = left == 2 || left == 4 ? 2 : 3;
            packed.push_back(
                group(Digit(bridge.begin() + i, bridge.begin() + i + count)));
            i += count;
        }
        return deep(a->prefix, join(a->middle, packed, b->middle), b->suffix);
    }

    static tuple<Digit, ItemPtr, Digit> split_digit(const Digit &d, size_t &k) {
        auto it = d.begin();
        while (k >= (*it)->size) {
            k -= (*it)->size;
            ++it;
        }
        return {Digit(d.begin(), it), *it, Digit(it + 1, d.end())};
    }

    static tuple<TreePtr, ItemPtr, TreePtr> split_tree(const TreePtr &t,
                                                       size_t &k) {
        if (t->single)
            return {TreePtr{}, t->single, TreePtr{}};
        size_t left_size = size(t->prefix);
        if (k < left_size) {
            auto [a, x, b] = split_digit(t->prefix, k);
            return {from_digit(a), x, with_left(move(b), t->middle, t->suffix)};
        }
        k -= left_size;
        size_t middle_size = size(t->middle);
        if (k < middle_size) {
            auto [a, bundle, b] = split_tree(t->middle, k);
            auto [left, x, right] = split_digit(bundle->children, k);
            return {with_right(t->prefix, move(a), move(left)), x,
                    with_left(move(right), move(b), t->suffix)};
        }
        k -= middle_size;
        auto [a, x, b] = split_digit(t->suffix, k);
        return {with_right(t->prefix, t->middle, move(a)), x, from_digit(b)};
    }

    static ItemPtr locate(const Digit &digit, size_t &k) {
        for (const auto &x : digit) {
            if (k < x->size)
                return x;
            k -= x->size;
        }
        return {};
    }

    static ItemPtr locate(const TreePtr &t, size_t &k) {
        if (t->single)
            return t->single;
        if (auto x = locate(t->prefix, k))
            return x;
        size_t middle_size = size(t->middle);
        if (k < middle_size) {
            auto x = locate(t->middle, k);
            return locate(x->children, k);
        }
        k -= middle_size;
        return locate(t->suffix, k);
    }

  public:
    FingerTree() = default;
    size_t size() const { return size(root); }
    bool empty() const { return !root; }

    FingerTree push_front(T value) const {
        auto x = make_shared<Item>(Item{1, move(value), {}});
        return FingerTree(prepend(root, move(x)));
    }

    FingerTree push_back(T value) const {
        auto x = make_shared<Item>(Item{1, move(value), {}});
        return FingerTree(append(root, move(x)));
    }

    FingerTree pop_front() const {
        assert(root);
        return FingerTree(take_first(root).second);
    }

    FingerTree pop_back() const {
        assert(root);
        return FingerTree(take_last(root).second);
    }

    FingerTree concat(const FingerTree &other) const {
        return FingerTree(join(root, {}, other.root));
    }

    pair<FingerTree, FingerTree> split(size_t k) const {
        assert(k <= size());
        if (!k)
            return {FingerTree{}, *this};
        if (k == size())
            return {*this, FingerTree{}};
        auto [a, x, b] = split_tree(root, k);
        return {FingerTree(move(a)), FingerTree(prepend(b, move(x)))};
    }

    const T &at(size_t i) const {
        assert(i < size());
        return *locate(root, i)->value;
    }
};
