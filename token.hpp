/*
 *  token.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "String.hpp"
#include <cstddef>
#include <cstdint>

namespace sapphire {
    enum class Type {
        // Core structural tracking markers
        IDENTIFIER,
        EOF_STREAM,
        
        // Keywords
        KEYWORD_IF,
        KEYWORD_ELSE,
        KEYWORD_LOOP,
        KEYWORD_SUBR,
        KEYWORD_VAR_SINT64,
        KEYWORD_VAR_UINT64,
        KEYWORD_VAR_FLOAT,
        KEYWORD_RETURN,
        
        // Literals
        LITERAL_SINT64,
        LITERAL_UINT64,
        LITERAL_FLOAT,
        
        // Operators
        PLUS,
        MINUS,
        TIMES,
        DIVIDE,
        MODULUS,
        LESS_THAN,
        GREATER_THAN,
        LESS_EQUAL,
        GREATER_EQUAL,
        COMPARE_EQUAL,
        LOGICAL_NOT,
        LOGICAL_OR,
        LOGICAL_XOR,
        LOGICAL_AND,
        ASSIGNMENT,
    };

    using type_t = Type;

    union Value {
        std::int64_t  sint64;
        std::uint64_t uint64;
        long double   float_val;
    };

    using value_t = Value;

    struct Token {
        type_t      type;
        value_t     value;
        String      lexeme;
        std::size_t line_count;
    };

    using token_t = Token;
}

#endif // end TOKEN_HPP
