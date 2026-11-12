/*
 *  utility.cpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#include "utility.hpp"
#include <cstdio>
#include <stdexcept>

namespace sapphire {
    void get_input(String& source, const char delimiter, bool& EOF_flag) {
        EOF_flag = false;
        source.clear();
        int c{std::getchar()};
        int dlim{static_cast<int>(delimiter)};
        while (c != dlim && c != EOF) {
            source.push_back(static_cast<char>(c));
            c = std::getchar();
        }
        if (c == EOF) {
            if (std::feof(stdin)) {
                EOF_flag = true;
            } else if (std::ferror(stdin)) {
                throw std::runtime_error("sapphire::get_input(): A runtime error has occurred!");
            }
        }
    }

    void print(const String& str) {
        int c{};
        for (std::size_t i{0}; i < str.length(); ++i) {
            c = std::putchar(static_cast<int>(str[i]));
            if (c == EOF) {
                if (std::ferror(stdout)) {
                    throw std::runtime_error("sapphire::print(): A runtime error has occurred!");
                }
                break;
            }
        }
    }

    long double chars_to_float(const String& digits) {
        if (digits.is_empty()) {
            throw std::invalid_argument("sapphire::chars_to_float(): Cannot convert empty string to float!");
        }
        char c;
        bool decimal_flag{false};
        long double integer_part{0.0L};
        long double fractional_part{0.0L};
        String buffer{};
        for (std::size_t i{0}; i < digits.length(); ++i) {
            c = digits[i];
            if (is_decimal_point(c)) {
                if (decimal_flag) {
                    throw std::invalid_argument("sapphire::chars_to_float(): Too many '.' chars detected!");
                }
                if (digits.length() == 1) {
                    // Since there is a decimal point and the length of digits is 1 this must be an error
                    throw std::invalid_argument("sapphire::chars_to_float(): A single '.' cannot constitute a number!");
                }
                decimal_flag = true;
                if (buffer.is_empty()) {
                    // Since there is a decimal point and length of digits is > 1
                    // then there cannot be any digits before the decimal point
                    continue;
                }
                integer_part = static_cast<long double>(chars_to_int(buffer));
                buffer.clear();
                continue;
            }
            if (c < '0' || c > '9') {
                throw std::invalid_argument("sapphire::chars_to_float(): Invalid argument. A digit was expected!");
            }
            buffer.push_back(c);
        }
        if (decimal_flag) {
            if (buffer.is_empty()) {
                // Since there is a decimal point and buffer is empty there were no digits after the decimal point
                return integer_part;
            }
            long double denominator{1.0L};
            for (std::size_t i{0}; i < buffer.length(); ++i) {
                denominator = denominator * 10.0L;
            }
            fractional_part = static_cast<long double>(chars_to_int(buffer));
            fractional_part = fractional_part / denominator;
        } else {
            // No decimal point or fractional part was input (fractional_part = 0.0L)
            integer_part = static_cast<long double>(chars_to_int(buffer));
        }
        return integer_part + fractional_part;
    }

    std::int64_t chars_to_int(const String& digits) {
        if (digits.is_empty()) {
            throw std::invalid_argument("sapphire::chars_to_int(): Cannot convert empty string to integer!");
        }
        char c;
        std::int64_t ans{0};
        for (std::size_t i{0}; i < digits.length(); ++i) {
            c = digits[i];
            if (c < '0' || c > '9') {
                throw std::invalid_argument("sapphire::chars_to_int(): Invalid argument. A digit was expected!");
            }
            if ((ans > max_value / 10) || ((ans == max_value / 10) && ((c - '0') > max_value % 10))) {
                throw std::out_of_range("sapphire::chars_to_int(): Value is too large!");
            }
            ans = ans * 10 + (c - '0');
        }
        return ans;
    }

    void get_identifier(String& identifier, const String& source, std::size_t& index) {
        if (index >= source.length()) {
            throw std::out_of_range("sapphire::get_identifier(): Index is out of bounds!");
        }
        if (!is_alpha(source[index]) && !is_underscore(source[index])) {
            throw std::invalid_argument("sapphire::get_identifier(): Identifiers cannot begin with a digit!");
        }
        identifier.clear();
        while (index < source.length() && (is_alpha(source[index]) || is_digit(source[index]) || is_underscore(source[index]))) {
            identifier.push_back(source[index]);
            ++index;
        }
    }

    void get_number(String& digits, const String& source, std::size_t& index, number_type_t& number_type) {
        digits.clear();
        number_type = number_type_t::UNSIGNED_INTEGER_TYPE;
        bool decimal_flag{false};
        while (index < source.length() && (is_digit(source[index]) || is_decimal_point(source[index]))) {
            if (is_decimal_point(source[index])) {
                if (decimal_flag) {
                    throw std::invalid_argument("sapphire::get_number(): Too many '.' chars detected!");
                }
                decimal_flag = true;
            }
            digits.push_back(source[index]);
            ++index;
        }
        if (digits.length() == 1 && decimal_flag) {
            throw std::invalid_argument("sapphire::get_number(): A single '.' cannot constitute a number!");
        }
        if (decimal_flag) {
            number_type = number_type_t::FLOAT_TYPE;
        }
    }

    bool is_alpha(const char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    bool is_decimal_point(const char c) {
        return c == '.';
    }

    bool is_digit(const char c) {
        return c >= '0' && c <= '9';
    }

    bool is_operator(const char c) {
        switch (c) {
            case '+': [[fallthrough]];
            case '-': [[fallthrough]];
            case '*': [[fallthrough]];
            case '/': [[fallthrough]];
            case '%': [[fallthrough]];
            case '^': [[fallthrough]];
            case '<': [[fallthrough]];
            case '>': [[fallthrough]];
            case '=': [[fallthrough]];
            case '!': [[fallthrough]];
            case '&': [[fallthrough]];
            case '|':
                return true;
            default:
                return false;
        }
    }

    bool is_underscore(const char c) {
        return c == '_';
    }
}
