/**
 * @brief 莫队配合 bitset（三个区间的多重集交集）
 * @complexity 每批 b 问的端点移动 O(n√b + b)，另有排序及每问 O(ceil(MaxN / 字长)) 次字操作
 * @index 区间 [l,r)，每问给三个区间
 * @idea 给值 x 留一段连续的位。窗口中 x 有 c 个，就点亮这一段的前 c 位。三个窗口按位与，保留的位数就是交集大小。
 * @usage auto removed = mo_three_range_deletions<100000>(a, queries);
 *   每问给三个 {l,r}，返回删多少项才能让三个多重集相同。
 * @pitfall MaxN 至少为数组长度。每批保留 batch_size 个答案 bitset，内存紧就调小批量。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <size_t MaxN>
vector<ll> mo_three_range_deletions(const vector<ll> &a,
                                    const vector<array<pair<int, int>, 3>> &queries,
                                    int batch_size = 2048) {
    assert(a.size() <= MaxN && batch_size > 0);
    int n = (int)a.size(), q = (int)queries.size();
    auto sorted = a;
    sort(sorted.begin(), sorted.end());
    // 保留所有重复项，给同一个值留够槽位。
    vector<int> base(n);
    for (int i = 0; i < n; i++)
        base[i] = lower_bound(sorted.begin(), sorted.end(), a[i]) - sorted.begin();

    vector<int> count(n);
    vector<ll> ans(q);
    struct Query {
        int l, r, id;
    };

    for (int start = 0; start < q;) {
        int size = min(batch_size, q - start);
        vector<Query> batch;
        vector<bitset<MaxN>> common(size);
        for (int id = 0; id < size; id++) {
            common[id].set();
            for (auto [l, r] : queries[start + id]) {
                batch.push_back({l, r, id});
                ans[start + id] += r - l;
            }
        }
        int block = max(1, (int)(n / sqrt(batch.size())));
        sort(batch.begin(), batch.end(), [&](const Query &x, const Query &y) {
            int b1 = x.l / block, b2 = y.l / block;
            if (b1 != b2)
                return b1 < b2;
            return b1 % 2 == 0 ? x.r < y.r : x.r > y.r;
        });

        bitset<MaxN> active;
        fill(count.begin(), count.end(), 0);
        int l = 0, r = 0;
        auto add = [&](int i) {
            int x = base[i];
            active.set(x + count[x]++);
        };
        auto del = [&](int i) {
            int x = base[i];
            active.reset(x + --count[x]);
        };
        for (const auto &item : batch) {
            while (l > item.l)
                add(--l);
            while (r < item.r)
                add(r++);
            while (l < item.l)
                del(l++);
            while (r > item.r)
                del(--r);
            common[item.id] &= active;
        }
        for (int id = 0; id < size; id++)
            ans[start + id] -= 3LL * (ll)common[id].count();
        start += size;
    }
    return ans;
}
