/**
 * @brief 自定义哈希（整数和整数 pair）
 * @complexity 每次哈希 O(1)
 * @index 整数按 64 位处理
 * @idea 用 SplitMix64 混合，再加运行时种子，降低输入专门构造碰撞的风险。
 * @usage unordered_map<long long, int, SplitMixHash> count;
 *   unordered_map<pair<int, int>, int, SplitMixHash> points;
 * @pitfall 每次运行种子可能不同。仍可能发生碰撞，最坏复杂度没有保证。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

struct SplitMixHash {
    static ull mix(ull x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(ull x) const {
        static const auto seed =
            chrono::steady_clock::now().time_since_epoch().count();
        return mix(x + ull(seed));
    }

    template <integral A, integral B> size_t operator()(const pair<A, B> &p) const {
        ull a = mix(ull(p.first));
        ull b = mix(ull(p.second));
        return (*this)(a ^ rotl(b, 1));
    }
};
