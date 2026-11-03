/**
 * @brief 可持久化块状数组
 * @complexity 修改 O(n/B + B)，读取 O(1)
 * @index 位置与版本号 0-based
 * @idea 一个版本存各块的共享指针。单点修改时复制指针表，再复制要改的那一块。
 * @usage PersistentBlocks<T> a(values); int v=a.set(old,i,x); a.get(v,i)。
 * @pitfall 版本 0 是初值，旧版本可继续分支。需要可复制的元素类型，默认块长取 sqrt(n)。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T> struct PersistentBlocks {
    using Block = shared_ptr<const vector<T>>;
    int n, width;
    vector<vector<Block>> versions;

    explicit PersistentBlocks(const vector<T> &a)
        : n((int)a.size()), width(max(1, (int)sqrt(n))) {
        auto &first = versions.emplace_back();
        for (int l = 0; l < n; l += width)
            first.push_back(make_shared<const vector<T>>(
                a.begin() + l, a.begin() + min(n, l + width)));
    }

    int set(int version, int i, T value) {
        assert(0 <= i && i < n);
        auto next = versions[version];
        int block = i / width;
        auto copy = make_shared<vector<T>>(*next[block]);
        (*copy)[i % width] = move(value);
        next[block] = move(copy);
        versions.push_back(move(next));
        return (int)versions.size() - 1;
    }

    const T &get(int version, int i) const {
        assert(0 <= i && i < n);
        return (*versions[version][i / width])[i % width];
    }
};
