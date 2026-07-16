#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_function(const string &s)
{
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; ++i)
    {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            j++;
        pi[i] = j;
    }
    return pi;
}

vector<int> kmp(const string &T, const string &S) // T是文章，S是单词
{
    string cur = S + "#" + T;
    int t = T.size(), s = S.size();
    vector<int> v;
    vector<int> pi = prefix_function(cur);
    for (int i = s + 1; i <= s + t; ++i)
        if (pi[i] == s)
            v.push_back(i - 2 * s);
    return v;
}

// 统计前缀出现次数
{
    for (int i = 0; i < n; ++i)
        ans[pi[i]]++;
    for (int i = n - 1; i > 0; --i)
        ans[pi[i - 1]] += ans[i];
    for (int i = 0; i <= n; ++i)
        ans[i]++;
}

// 最小表示法
int minrep(const string &s)
{
    int n = s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n)
    {
        if (i == j)
            j++;
        if (s[(i + k) % n] == s[(j + k) % n])
            k++;
        else if (s[(i + k) % n] > s[(j + k) % n])
        {
            i = i + k + 1;
            k = 0;
        }
        else
        {
            j = j + k + 1;
            k = 0;
        }
    }
    return min(i, j);
}

// exkmp
vector<int> z_function(const string &s)
{
    int n = s.size();
    vector<int> z(n);
    z[0] = n; // 或者 z[0] = 0;
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i)
    {
        if (i <= r && z[i - l] < r - i + 1)
            z[i] = z[i - l];
        else
        {
            z[i] = max(0, r - i + 1);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                ++z[i];
        }
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}

// Trie
const int MAXN = 3e6 + 5;
struct trie
{
    int nxt[MAXN][65] = {}, cnt = 0;
    int vis[MAXN] = {};

    void clear()
    {
        for (int i = 0; i <= cnt; ++i)
        {
            for (int j = 0; j < 65; ++j)
                nxt[i][j] = 0;
            vis[i] = 0;
        }
        cnt = 0;
    }
    int getord(char c)
    {
        if (c <= 'z' && c >= 'a')
            return c - 'a';
        else if (c <= 'Z' && c >= 'A')
            return c - 'A' + 26;
        else
            return c - '0' + 52;
    }
    void insert(string s)
    {
        int p = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            int c = getord(s[i]);
            if (!nxt[p][c])
                nxt[p][c] = ++cnt;
            p = nxt[p][c];
            vis[p]++;
        }
    }

    int find(string s)
    {
        int p = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            int c = getord(s[i]);
            if (!nxt[p][c])
                return 0;
            p = nxt[p][c];
        }
        return vis[p];
    }
};