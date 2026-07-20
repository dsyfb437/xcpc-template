// Segment Tree
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll NEG_INF = LLONG_MIN / 4; // 查询最大值时用于无交集区间的返回值

struct SegTree
{
    struct Node
    {
        ll sum, maxx;
        ll mul, add;
        ll len;
    };

    int n;
    vector<Node> tree;

    // 构造函数，传入数组大小和初始值（下标从1开始）
    SegTree(int n_, const vector<ll> &init)
    {
        n = n_;
        tree.resize(4 * n + 5);
        build(1, 1, n, init);
    }

    // 建树
    void build(int p, int l, int r, const vector<ll> &init)
    {
        tree[p].mul = 1;
        tree[p].add = 0;
        tree[p].len = r - l + 1; // 区间长度
        if (l == r)
        {
            tree[p].sum = tree[p].maxx = init[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, init);
        build(p << 1 | 1, mid + 1, r, init);
        pushup(p);
    }

    void pushup(int p)
    {
        tree[p].sum = tree[p << 1].sum + tree[p << 1 | 1].sum;
        tree[p].maxx = max(tree[p << 1].maxx, tree[p << 1 | 1].maxx);
    }

    // 对节点 p 应用乘法 mul 和加法 add
    void apply(int p, ll mul, ll add)
    {
        tree[p].sum = tree[p].sum * mul + add * tree[p].len;
        tree[p].maxx = tree[p].maxx * mul + add;

        tree[p].mul = tree[p].mul * mul;
        tree[p].add = tree[p].add * mul + add;
    }

    // 下传懒标记
    void pushdown(int p)
    {
        if (tree[p].mul == 1 && tree[p].add == 0)
            return; // 无标记，可提前返回（非必须）
        apply(p << 1, tree[p].mul, tree[p].add);
        apply(p << 1 | 1, tree[p].mul, tree[p].add);
        tree[p].mul = 1;
        tree[p].add = 0;
    }

    // 区间更新（乘 mul，加 add）
    void update(int L, int R, ll mul, ll add)
    {
        update(1, 1, n, L, R, mul, add);
    }

    void update(int p, int l, int r, int ql, int qr, ll mul, ll add)
    {
        if (ql <= l && r <= qr)
        {
            apply(p, mul, add);
            return;
        }
        if (ql > r || qr < l)
            return; // 不重叠
        pushdown(p);
        int mid = (l + r) >> 1;
        update(p << 1, l, mid, ql, qr, mul, add);
        update(p << 1 | 1, mid + 1, r, ql, qr, mul, add);
        pushup(p);
    }

    // 区间求和
    ll query_sum(int L, int R)
    {
        return query_sum(1, 1, n, L, R);
    }

    ll query_sum(int p, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr)
            return tree[p].sum;
        if (ql > r || qr < l)
            return 0;
        pushdown(p);
        int mid = (l + r) >> 1;
        return query_sum(p << 1, l, mid, ql, qr) +
               query_sum(p << 1 | 1, mid + 1, r, ql, qr);
    }

    // 区间最大值
    ll query_max(int L, int R)
    {
        return query_max(1, 1, n, L, R);
    }

    ll query_max(int p, int l, int r, int ql, int qr)
    {
        if (ql <= l && r <= qr)
            return tree[p].maxx;
        if (ql > r || qr < l)
            return NEG_INF;
        pushdown(p);
        int mid = (l + r) >> 1;
        return max(query_max(p << 1, l, mid, ql, qr),
                   query_max(p << 1 | 1, mid + 1, r, ql, qr));
    }
};

// ========== 使用示例 ==========
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];

    SegTree seg(n, a);

    while (m--)
    {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1)
        { // 区间加
            ll x;
            cin >> x;
            seg.update(l, r, 1, x); // 乘 1，加 x
        }
        else if (op == 2)
        { // 区间乘
            ll x;
            cin >> x;
            seg.update(l, r, x, 0); // 乘 x，加 0
        }
        else if (op == 3)
        { // 区间求和
            cout << seg.query_sum(l, r) << '\n';
        }
        else
        { // 区间最大值
            cout << seg.query_max(l, r) << '\n';
        }
    }

    return 0;
}