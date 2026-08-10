ll quickpow(ll a, ll b)
{
    ll res = 1;
    a %= MOD;
    while (b)
    {
        if (b & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}


// exgcd
ll Exgcd(ll a, ll b, ll &x, ll &y)
{
    if (!b)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll d = Exgcd(b, a % b, x, y);
    ll t = x;
    x = y;
    y = t - (a / b) * y;
    return d;
}

const double EPS = 1e-9;

// 高斯消元求解线性方程组 A*z = b
// A 是 n×m 矩阵，b 是长度为 n 的向量
// 返回解向量（长度为 m），若没有唯一解则返回空向量
vector<double> gauss(vector<vector<double>> A, vector<double> b) {
    int n = A.size(), m = A[0].size();
    // 构造增广矩阵
    for (int i = 0; i < n; ++i) {
        A[i].push_back(b[i]);
    }

    int row = 0;
    vector<int> where(m, -1);
    for (int col = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row; i < n; ++i) {
            if (fabs(A[i][col]) > fabs(A[sel][col])) sel = i;
        }
        if (fabs(A[sel][col]) < EPS) continue; 
        swap(A[sel], A[row]);
        where[col] = row;

        for (int i = 0; i < n; ++i) {
            if (i != row) {
                double factor = A[i][col] / A[row][col];
                for (int j = col; j <= m; ++j) {
                    A[i][j] -= factor * A[row][j];
                }
            }
        }
        ++row;
    }

    for (int i = row; i < n; ++i) {
        if (fabs(A[i][m]) > EPS) return {}; 
    }

    vector<double> z(m, 0);
    for (int col = 0; col < m; ++col) {
        if (where[col] != -1) {
            z[col] = A[where[col]][m] / A[where[col]][col];
        }
    }
    return z;
}

// 拉格朗日插值：给定 n 个点 (x[i], y[i])，计算在 k 处的插值结果
//O(n^2)
ll lagrange_interpolation(const vector<ll>& x, const vector<ll>& y, int n, ll k) {
    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll up = 1, down = 1;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            up = up * ((k - x[j] + MOD) % MOD) % MOD;
            down = down * ((x[i] - x[j] + MOD) % MOD) % MOD;
        }
        ans = (ans + y[i] * up % MOD * quickpow(down, MOD - 2) % MOD) % MOD;
    }
    return ans;
}