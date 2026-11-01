/**
 * @brief 提交模板
 * @complexity -
 * @index -
 * @idea 现场赛提交骨架：调试宏、多测开关、板子区。
 * @usage 改 `solve()` 和 `namespace lib` 里的板子区。
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

// ---------- 板子区：要用什么就贴什么 ----------
namespace lib {
// struct DSU { ... };
// struct Fenwick { ... };
} // namespace lib
using namespace lib;

void solve() {}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(12);
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
