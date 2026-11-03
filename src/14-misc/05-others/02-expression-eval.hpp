/**
 * @brief 整数表达式求值
 * @complexity O(n) 时间，递归栈深度等于括号嵌套深度
 * @index 支持括号、一元 + - 和二元 + - * / %
 * @idea 递归下降分三层。先读一个数或括号，再处理乘除取模，最后处理加减。
 * @usage long long value = evaluate_expression("2 * -(3 + 4)");  // -14
 * @pitfall 除法向 0 取整。遇到语法错误、除零或超出 long long 会抛异常。需要 __int128。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace expression_detail {
struct Parser {
    using Wide = __int128;
    static constexpr ll low = numeric_limits<ll>::min();
    static constexpr ll high = numeric_limits<ll>::max();

    string_view text;
    size_t pos = 0;

    char peek() {
        while (pos < text.size() && isspace((unsigned char)text[pos]))
            pos++;
        return pos < text.size() ? text[pos] : '\0';
    }

    static ll narrow(Wide value) {
        if (value < low || value > high)
            throw overflow_error("integer overflow");
        return (ll)value;
    }

    ll factor() {
        bool negative = false;
        char c = peek();
        while (c == '+' || c == '-') {
            if (c == '-')
                negative = !negative;
            pos++;
            c = peek();
        }

        Wide value = 0;
        if (c == '(') {
            pos++;
            value = expression();
            if (peek() != ')')
                throw invalid_argument("expected ')'");
            pos++;
        } else {
            if (c < '0' || c > '9')
                throw invalid_argument("expected number");
            while (pos < text.size() && '0' <= text[pos] && text[pos] <= '9') {
                value = value * 10 + text[pos++] - '0';
                if (value > Wide(high) + 1)
                    throw overflow_error("literal");
            }
        }
        return narrow(negative ? -value : value);
    }

    ll term() {
        ll value = factor();
        for (char op = peek(); op == '*' || op == '/' || op == '%'; op = peek()) {
            pos++;
            Wide rhs = factor();
            if (op != '*' && rhs == 0)
                throw domain_error("division by zero");
            if (op == '*')
                value = narrow(value * rhs);
            if (op == '/')
                value = narrow(value / rhs);
            if (op == '%')
                value = narrow(value % rhs);
        }
        return value;
    }

    ll expression() {
        ll value = term();
        for (char op = peek(); op == '+' || op == '-'; op = peek()) {
            pos++;
            Wide rhs = term();
            value = narrow(op == '+' ? value + rhs : value - rhs);
        }
        return value;
    }
};
} // namespace expression_detail

inline ll evaluate_expression(string_view text) {
    expression_detail::Parser parser{text};
    ll value = parser.expression();
    parser.peek();
    if (parser.pos != text.size())
        throw invalid_argument("unexpected token");
    return value;
}
