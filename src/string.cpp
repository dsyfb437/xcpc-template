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
int minrep(string s)
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