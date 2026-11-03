/**
 * @brief 快读快写（整数）
 * @complexity 每个整数 O(位数)
 * @index 按空白分隔
 * @idea 用 fread 和 fwrite 成块读写。read 返回 false 表示读完或遇到不合法的整数。
 * @usage FastInput in;
 *   FastOutput out;
 *   long long x;
 *   while (in.read(x)) out.write(x);
 * @pitfall 同一输入流别混着用 cin 或 scanf。输出默认带换行，析构会刷新。需要立即显示时用 flush。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct FastInput {
    explicit FastInput(FILE *file = stdin) : file(file) {}

    template <integral T>
        requires(!same_as<T, bool>) bool
    read(T &result) {
        int c = get();
        while (c != EOF && c <= ' ')
            c = get();
        if (c == EOF)
            return false;

        bool negative = c == '-';
        if (negative || c == '+')
            c = get();
        using U = make_unsigned_t<T>;
        U limit = numeric_limits<T>::max();
        if constexpr (is_signed_v<T>) {
            if (negative)
                ++limit;
        }
        bool valid = c >= '0' && c <= '9' && (!negative || is_signed_v<T>);
        U value = 0;
        while (c != EOF && c > ' ') {
            if ('0' <= c && c <= '9') {
                U digit = c - '0';
                if (value > limit / 10 ||
                    (value == limit / 10 && digit > limit % 10)) {
                    valid = false;
                } else {
                    value = value * 10 + digit;
                }
            } else {
                valid = false;
            }
            c = get();
        }
        if (!valid)
            return false;
        if (negative)
            value = U(0) - value;
        result = static_cast<T>(value);
        return true;
    }

  private:
    FILE *file;
    unsigned char buffer[1 << 16];
    size_t pos = 0, length = 0;

    int get() {
        if (pos == length) {
            pos = 0;
            length = fread(buffer, 1, sizeof buffer, file);
            if (length == 0)
                return EOF;
        }
        return buffer[pos++];
    }
};

struct FastOutput {
    explicit FastOutput(FILE *file = stdout) : file(file) {}
    FastOutput(const FastOutput &) = delete;
    FastOutput &operator=(const FastOutput &) = delete;
    ~FastOutput() { flush(); }

    template <integral T>
        requires(!same_as<T, bool>)
    void write(T value, char separator = '\n') {
        char digits[numeric_limits<T>::digits10 + 3];
        auto [end, error] = to_chars(digits, digits + sizeof digits, value);
        for (const char *p = digits; p != end; p++)
            put(*p);
        if (separator)
            put(separator);
    }

    void flush() {
        write_buffer();
        fflush(file);
    }

  private:
    FILE *file;
    char buffer[1 << 16];
    size_t length = 0;

    void write_buffer() {
        fwrite(buffer, 1, length, file);
        length = 0;
    }

    void put(char c) {
        if (length == sizeof buffer)
            write_buffer();
        buffer[length++] = c;
    }
};
