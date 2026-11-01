/**
 * @brief 并查集
 * @complexity 近似 O(1)
 * @index 下标 1-based
 *
 * 路径压缩 + 按大小合并，下标从 1 开始。
 * cnt 是当前连通块个数，size(x) 是 x 所在块的大小。
 *
 * 例：
 *   DSU dsu(n);
 *   dsu.unite(u, v);
 *   if (dsu.same(u, v)) { ... } */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DSU {
    vector<int> p, sz;
    int cnt;

    explicit DSU(int n) : p(n + 1), sz(n + 1, 1), cnt(n) {
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b)
            return false;
        if (sz[a] < sz[b])
            swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        cnt--;
        return true;
    }

    bool same(int a, int b) { return find(a) == find(b); }
    int size(int x) { return sz[find(x)]; }
};
