/**
 * @brief 离线区间不同数个数
 * @complexity O((n+q) log(2+n+q)) 时间，O(n+q) 空间
 * @index 查询 [l,r)，0-based
 * @idea 询问按右端点排好。从左往右扫，树状数组只保留每个值最后出现的位置。
 * @usage auto count = offline_distinct_counts(a, queries);
 *   queries 每项是 {l,r}，答案按原询问顺序返回。
 * @pitfall 适用于没有修改的数组。空区间答案为 0。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<int> offline_distinct_counts(const vector<ll> &a,
                                           const vector<pair<int, int>> &queries) {
    int n = (int)a.size();
    auto values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());

    vector<int> order(queries.size()), answer(queries.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int i, int j) { return queries[i].second < queries[j].second; });

    vector<int> last(values.size(), -1), bit(n + 1);
    auto add = [&](int pos, int delta) {
        for (int i = pos + 1; i <= n; i += i & -i)
            bit[i] += delta;
    };
    auto prefix = [&](int end) {
        int total = 0;
        for (int i = end; i > 0; i -= i & -i)
            total += bit[i];
        return total;
    };

    int pos = 0;
    for (int id : order) {
        auto [l, r] = queries[id];
        while (pos < r) {
            int v =
                lower_bound(values.begin(), values.end(), a[pos]) - values.begin();
            if (last[v] != -1)
                add(last[v], -1);
            add(pos, 1);
            last[v] = pos++;
        }
        answer[id] = prefix(r) - prefix(l);
    }
    return answer;
}
