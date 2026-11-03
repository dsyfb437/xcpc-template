/**
 * @brief PQ 树（连续区间约束）
 * @complexity 每条约束期望 O(n + k)，k 是输入组长度
 * @index 元素 0 到 n - 1
 * @idea P 节点儿子可重排，Q 节点只可整体反向。每次把选中元素收成一块，非根的混合子树要把选中块贴到端点。
 * @usage PQTree t(n); t.constrain(group) 要求 group 连续，成功返回 true。t.frontier() 取一个可行排列。
 * @pitfall 同一组可重复给出元素。失败时保留先前约束。此版每次扫描整树，适合中小规模。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class PQTree {
    enum Kind { leaf, p_node, q_node };
    struct Node;
    using Ptr = shared_ptr<const Node>;
    struct Node {
        Kind kind;
        int id, size;
        vector<Ptr> children;
    };

    Ptr root;
    int n;

    static Ptr make(Kind kind, vector<Ptr> children) {
        if (children.empty())
            return {};
        if (children.size() == 1)
            return children.front();
        int size = 0;
        for (const auto &u : children)
            size += u->size;
        return make_shared<Node>(Node{kind, -1, size, move(children)});
    }

    struct Reduction {
        enum State { empty, mixed, full };
        using Strip = list<Ptr>;
        unordered_map<const Node *, int> chosen;

        int count(const Ptr &u, const vector<bool> &marked) {
            int answer = 0;
            if (u->kind == leaf) {
                answer = marked[u->id];
            } else {
                for (const auto &v : u->children)
                    answer += count(v, marked);
            }
            chosen.emplace(u.get(), answer);
            return answer;
        }

        State state(const Ptr &u) const {
            int selected = chosen.at(u.get());
            if (!selected)
                return empty;
            return selected == u->size ? full : mixed;
        }

        // 返回从空侧到满侧的有序条带，供父节点拼接。
        optional<Strip> boundary(const Ptr &u) const {
            Strip result;
            if (u->kind == p_node) {
                vector<Ptr> before, after;
                Ptr partial;
                for (const auto &v : u->children) {
                    State s = state(v);
                    if (s == empty)
                        before.push_back(v);
                    else if (s == full)
                        after.push_back(v);
                    else if (partial)
                        return nullopt;
                    else
                        partial = v;
                }
                if (auto group = make(p_node, move(before)))
                    result.push_back(group);
                if (partial) {
                    auto side = boundary(partial);
                    if (!side)
                        return nullopt;
                    result.splice(result.end(), *side);
                }
                if (auto group = make(p_node, move(after)))
                    result.push_back(group);
                return result;
            }

            auto order = u->children;
            auto fits = [&] {
                State last = empty;
                for (const auto &v : order) {
                    State s = state(v);
                    if (s < last || (s == mixed && last == mixed))
                        return false;
                    last = s;
                }
                return true;
            };
            if (!fits()) {
                reverse(order.begin(), order.end());
                if (!fits())
                    return nullopt;
            }
            for (const auto &v : order) {
                if (state(v) != mixed) {
                    result.push_back(v);
                } else {
                    auto side = boundary(v);
                    if (!side)
                        return nullopt;
                    result.splice(result.end(), *side);
                }
            }
            return result;
        }

        Ptr reduce(const Ptr &u) const {
            int total = chosen.at(u.get());
            if (total == 0 || total == u->size)
                return u;
            auto children = u->children;
            for (auto &v : children)
                if (chosen.at(v.get()) == total) {
                    // 所有选中叶子都在同一儿子里，只缩那一棵子树。
                    v = reduce(v);
                    return v ? make(u->kind, move(children)) : Ptr{};
                }

            if (u->kind == p_node) {
                vector<Ptr> outside, inside, partial;
                for (const auto &v : children) {
                    State s = state(v);
                    if (s == empty)
                        outside.push_back(v);
                    else if (s == full)
                        inside.push_back(v);
                    else
                        partial.push_back(v);
                }
                if (partial.size() > 2)
                    return {};
                Strip block;
                if (!partial.empty()) {
                    auto left = boundary(partial.front());
                    if (!left)
                        return {};
                    block.splice(block.end(), *left);
                }
                if (auto group = make(p_node, move(inside)))
                    block.push_back(group);
                if (partial.size() == 2) {
                    auto right = boundary(partial.back());
                    if (!right)
                        return {};
                    right->reverse();
                    block.splice(block.end(), *right);
                }
                outside.push_back(make(q_node, {block.begin(), block.end()}));
                return make(p_node, move(outside));
            }

            int l = 0, r = (int)children.size() - 1;
            while (state(children[l]) == empty)
                l++;
            while (state(children[r]) == empty)
                r--;
            Strip order;
            for (int i = 0; i < (int)children.size(); i++) {
                const auto &v = children[i];
                State s = state(v);
                if (i < l || i > r || s == full) {
                    order.push_back(v);
                } else {
                    if (s == empty || (i != l && i != r))
                        return {};
                    auto side = boundary(v);
                    if (!side)
                        return {};
                    if (i == r)
                        side->reverse();
                    order.splice(order.end(), *side);
                }
            }
            return make(q_node, {order.begin(), order.end()});
        }
    };

  public:
    explicit PQTree(int n) : n(n) {
        vector<Ptr> elements;
        for (int i = 0; i < n; i++)
            elements.push_back(make_shared<Node>(Node{leaf, i, 1, {}}));
        root = make(p_node, move(elements));
    }

    bool constrain(const vector<int> &group) {
        if (!root)
            return group.empty();
        vector<bool> marked(n);
        for (int u : group) {
            assert(0 <= u && u < n);
            marked[u] = true;
        }
        Reduction work;
        work.count(root, marked);
        auto next = work.reduce(root);
        if (!next)
            return false;
        root = move(next);
        return true;
    }

    vector<int> frontier() const {
        vector<int> result;
        auto dfs = [&](auto &&self, const Ptr &u) -> void {
            if (!u)
                return;
            if (u->kind == leaf) {
                result.push_back(u->id);
            } else {
                for (const auto &v : u->children)
                    self(self, v);
            }
        };
        dfs(dfs, root);
        return result;
    }
};
