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