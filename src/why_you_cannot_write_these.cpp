// DSU
struct DSU
{
    vector<int> p, sz;
    DSU(int n)
    {
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }

    void unite(int a, int b)
    {
        a = find(a), b = find(b);
        if (a == b)
            return;
        if (sz[a] < sz[b])
            swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
    }

    bool same(int a, int b) { return find(a) == find(b); }
};

// e.g.
DSU dsu(n);
dsu.unite(1, 2);
if (dsu.same(1, 3))
{
    ...
}