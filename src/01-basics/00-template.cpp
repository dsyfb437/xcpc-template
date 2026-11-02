/**
 * @brief 提交模板
 * @complexity -
 * @index -
 * 
 * @verified
 * @print-boilerplate
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using ld = long double;

// #define MULTI_TEST

#ifndef ONLINE_JUDGE
#define LOCAL
#endif

#ifdef LOCAL
template <class... Args> void debug_out(const Args &...args) {
    ((cerr << args << ", "), ...);
}
#define debug(...) \
    do { \
        cerr << "[" << #__VA_ARGS__ << "] = "; \
        debug_out(__VA_ARGS__); \
        cerr << "\n"; \
    } while (0)
#else
#define debug(...) ((void)0)
#endif

namespace lib {} // namespace lib
using namespace lib;

void solve() {}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // cout << fixed << setprecision(12);
    // freopen("in.txt", "r", stdin);
    // freopen("out.txt", "w", stdout);

    int T = 1;
#ifdef MULTI_TEST
    cin >> T;
#endif
    while (T--)
        solve();
    return 0;
}
