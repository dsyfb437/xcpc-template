/**
 * @brief 字符串哈希
 * @complexity 预处理 O(n)，查询 O(1)
 * @index 字符串下标 0-based
 *
 * 双模数 + 固定底数。sub(l, r) 返回 s[l..r) 的哈希，比较两段直接比 pair。
 * 坑：固定底数理论上能被构造数据卡；对拍或多人赛可把底数改成随机值。
 *
 * 例：
 *   Hash h(s);
 *   if (h.sub(l1, r1) == h.sub(l2, r2)) 两段相等；
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Hash {
    static constexpr ll M1 = 1000000007, M2 = 1000000009;
    static constexpr ll B1 = 911382323, B2 = 972663749;

    vector<ll> h1, h2, p1, p2;

    explicit Hash(const string &s) {
        int n = s.size();
        h1.assign(n + 1, 0), h2.assign(n + 1, 0);
        p1.assign(n + 1, 1), p2.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * B1 + s[i]) % M1;
            h2[i + 1] = (h2[i] * B2 + s[i]) % M2;
            p1[i + 1] = p1[i] * B1 % M1;
            p2[i + 1] = p2[i] * B2 % M2;
        }
    }

    pair<ll, ll> sub(int l, int r) const { // s[l..r)
        ll a = (h1[r] - h1[l] * p1[r - l] % M1 + M1) % M1;
        ll b = (h2[r] - h2[l] * p2[r - l] % M2 + M2) % M2;
        return {a, b};
    }
};
