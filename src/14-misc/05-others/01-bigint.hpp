/**
 * @brief 大整数（非负）
 * @complexity 加减、除以小整数 O(n)，乘法 O(nm)，n、m 为块数
 * @index 每块 9 位十进制，低位在前
 * @idea 按 10^9 存块，进位和借位跟竖式一样。
 * @usage BigInt a("12345678901234567890"), b(123);
 *   string product = (a * b).str();
 *   auto [quotient, remainder] = a.divmod(97);
 * @pitfall 字符串只传非负十进制数字。减法要求左边不小于右边，除数必须为正。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

class BigInt {
    static constexpr int base = 1000000000;
    vector<int> digits;

    void trim() {
        while (!digits.empty() && digits.back() == 0)
            digits.pop_back();
    }

  public:
    BigInt(ull value = 0) {
        while (value > 0) {
            digits.push_back(value % base);
            value /= base;
        }
    }

    explicit BigInt(string_view s) {
        while (!s.empty()) {
            size_t first = s.size() > 9 ? s.size() - 9 : 0;
            int block = 0;
            for (size_t i = first; i < s.size(); i++) {
                assert('0' <= s[i] && s[i] <= '9');
                block = block * 10 + s[i] - '0';
            }
            digits.push_back(block);
            s = s.substr(0, first);
        }
        trim();
    }

    string str() const {
        if (digits.empty())
            return "0";
        string out = to_string(digits.back());
        for (int i = (int)digits.size() - 2; i >= 0; i--) {
            string block = to_string(digits[i]);
            out.append(9 - block.size(), '0');
            out += block;
        }
        return out;
    }

    bool operator==(const BigInt &) const = default;

    strong_ordering operator<=>(const BigInt &other) const {
        if (digits.size() != other.digits.size())
            return digits.size() <=> other.digits.size();
        return lexicographical_compare_three_way(digits.rbegin(), digits.rend(),
                                                 other.digits.rbegin(),
                                                 other.digits.rend());
    }

    friend BigInt operator+(const BigInt &a, const BigInt &b) {
        BigInt out;
        size_t n = max(a.digits.size(), b.digits.size());
        int carry = 0;
        for (size_t i = 0; i < n; i++) {
            int sum = carry;
            if (i < a.digits.size())
                sum += a.digits[i];
            if (i < b.digits.size())
                sum += b.digits[i];
            out.digits.push_back(sum % base);
            carry = sum / base;
        }
        if (carry)
            out.digits.push_back(carry);
        return out;
    }

    friend BigInt operator-(const BigInt &a, const BigInt &b) {
        assert(a >= b);
        BigInt out = a;
        int borrow = 0;
        for (size_t i = 0; i < a.digits.size(); i++) {
            int value = a.digits[i] - borrow;
            if (i < b.digits.size())
                value -= b.digits[i];
            borrow = value < 0;
            out.digits[i] = value + borrow * base;
        }
        out.trim();
        return out;
    }

    friend BigInt operator*(const BigInt &a, const BigInt &b) {
        if (a.digits.empty() || b.digits.empty())
            return {};
        BigInt out;
        out.digits.resize(a.digits.size() + b.digits.size());
        for (size_t i = 0; i < a.digits.size(); i++) {
            ll carry = 0;
            for (size_t j = 0; j < b.digits.size(); j++) {
                ll value =
                    out.digits[i + j] + carry + 1LL * a.digits[i] * b.digits[j];
                out.digits[i + j] = value % base;
                carry = value / base;
            }
            out.digits[i + b.digits.size()] = carry;
        }
        out.trim();
        return out;
    }

    pair<BigInt, int> divmod(int divisor) const {
        assert(divisor > 0);
        BigInt out = *this;
        ll remainder = 0;
        for (size_t i = digits.size(); i-- > 0;) {
            ll value = remainder * base + digits[i];
            out.digits[i] = value / divisor;
            remainder = value % divisor;
        }
        out.trim();
        return {move(out), (int)remainder};
    }
};
