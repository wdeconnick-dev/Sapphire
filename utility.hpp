/*
 *  utility.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "parameters.hpp"
#include "String.hpp"
#include <cstdint>
#include <limits>

namespace sapphire {

    constexpr std::int64_t max_value{std::numeric_limits<std::int64_t>::max()};

    enum class NumberType {
        FLOAT_TYPE,
        SIGNED_INTEGER_TYPE,
        UNSIGNED_INTEGER_TYPE,
    };

    using number_type_t = NumberType;

    // I/O functions
    void get_input(String& source, char delimiter, bool& EOF_flag);
    void print(const String& source);

    // Conversion functions
    long double chars_to_float(const String& digits);
    std::int64_t chars_to_int(const String& digits);

    // Extraction functions
    void get_identifier(String& identifier, const String& source, std::size_t& index);
    void get_number(String& digits, const String& source, std::size_t& index, number_type_t& number_type);

    // Verification functions
    bool is_alpha(char c);
    bool is_decimal_point(char c);
    bool is_digit(char c);
    bool is_operator(char c);
    bool is_underscore(char c);
}

#endif // end UTILITY_HPP
