/**
 * @brief 分数类（精确运算）
 * @complexity 加减乘除 O(log M)，比较 O(1)
 * @index 分母始终为正，分子分母互质
 * @idea 交叉相乘用 __int128，中间结果约分后再转回 long long。
 * @usage Fraction a(1, 3), b(1, 6);
 *   auto c = a + b;  // c.num() == 1，c.den() == 2
 * @pitfall 分母为 0 或除以 0 会抛异常。约分后超出 long long 也会抛异常。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class Fraction {
    using Wide = __int128;
    ll numerator = 0, denominator = 1;

    static Fraction reduced(Wide n, Wide d) {
        if (d == 0)
            throw domain_error("zero denominator");
        if (d < 0) {
            n = -n;
            d = -d;
        }
        Wide a = n < 0 ? -n : n, b = d;
        while (b != 0) {
            Wide r = a % b;
            a = b;
            b = r;
        }
        n /= a;
        d /= a;
        if (n < numeric_limits<ll>::min() || n > numeric_limits<ll>::max() ||
            d > numeric_limits<ll>::max())
            throw overflow_error("fraction overflow");
        Fraction result;
        result.numerator = (ll)n;
        result.denominator = (ll)d;
        return result;
    }

  public:
    Fraction() = default;
    Fraction(ll n, ll d = 1) { *this = reduced(n, d); }
    ll num() const { return numerator; }
    ll den() const { return denominator; }

    bool operator==(const Fraction &) const = default;
    strong_ordering operator<=>(const Fraction &b) const {
        return (Wide(num()) * b.den()) <=> (Wide(b.num()) * den());
    }

    friend Fraction operator+(const Fraction &a, const Fraction &b) {
        Wide n = Wide(a.num()) * b.den() + Wide(b.num()) * a.den();
        return reduced(n, Wide(a.den()) * b.den());
    }

    friend Fraction operator-(const Fraction &a, const Fraction &b) {
        Wide n = Wide(a.num()) * b.den() - Wide(b.num()) * a.den();
        return reduced(n, Wide(a.den()) * b.den());
    }

    friend Fraction operator*(const Fraction &a, const Fraction &b) {
        return reduced(Wide(a.num()) * b.num(), Wide(a.den()) * b.den());
    }

    friend Fraction operator/(const Fraction &a, const Fraction &b) {
        return reduced(Wide(a.num()) * b.den(), Wide(a.den()) * b.num());
    }
};
