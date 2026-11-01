#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"

#include <bits/stdc++.h>
#include "../../alg/05-number/sieve.hpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long a, b;
    cin >> n >> a >> b;
    Sieve sieve(n);
    int pi = sieve.primes.size();
    vector<int> picked;
    if (a > 0)
        for (long long i = b; i < pi; i += a) picked.push_back(sieve.primes[i]);
    cout << pi << ' ' << picked.size() << '\n';
    for (int i = 0; i < (int)picked.size(); i++)
        cout << picked[i] << (i + 1 == (int)picked.size() ? '\n' : ' ');
}
