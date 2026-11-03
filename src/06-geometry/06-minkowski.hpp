/**
 * @brief 凸多边形闵可夫斯基和
 * @complexity O(n + m)
 * @index 顶点下标 0-based
 * @idea 从最低点起，两条凸包的边向量都按极角排列。归并边向量，同向时合成一条。
 * @usage minkowski_sum(a, b) 返回点集两两相加后的凸包，支持单点和线段。
 * @pitfall 输入为逆时针凸包，首点不重复，去掉共线中间点。坐标绝对值不超过 10^12。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline vector<pair<ll, ll>> minkowski_sum(vector<pair<ll, ll>> a,
                                          vector<pair<ll, ll>> b) {
    using P = pair<ll, ll>;
    if (a.empty() || b.empty())
        return {};
    auto by_y = [](P x, P y) {
        return pair{x.second, x.first} < pair{y.second, y.first};
    };
    auto edge_list = [&](vector<P> &p) {
        rotate(p.begin(), min_element(p.begin(), p.end(), by_y), p.end());
        vector<P> edges;
        if (p.size() > 1)
            for (int i = 0; i < (int)p.size(); i++) {
                auto [x, y] = p[i];
                auto [u, v] = p[(i + 1) % p.size()];
                edges.emplace_back(u - x, v - y);
            }
        return edges;
    };
    auto left = edge_list(a), right = edge_list(b);
    auto half = [](P p) { return p.second < 0 || (p.second == 0 && p.first < 0); };
    auto less_angle = [&](P u, P v) {
        if (half(u) != half(v))
            return half(u) < half(v);
        return (__int128)u.first * v.second > (__int128)u.second * v.first;
    };
    P pos{a[0].first + b[0].first, a[0].second + b[0].second};
    vector<P> answer{pos};
    for (int i = 0, j = 0; i < (int)left.size() || j < (int)right.size();) {
        P edge;
        if (j == (int)right.size() ||
            (i < (int)left.size() && less_angle(left[i], right[j]))) {
            edge = left[i++];
        } else if (i == (int)left.size() || less_angle(right[j], left[i])) {
            edge = right[j++];
        } else {
            edge = {left[i].first + right[j].first,
                    left[i].second + right[j].second};
            i++;
            j++;
        }
        pos.first += edge.first;
        pos.second += edge.second;
        if (i != (int)left.size() || j != (int)right.size())
            answer.push_back(pos);
    }
    return answer;
}
