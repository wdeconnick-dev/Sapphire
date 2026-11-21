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
        KEYWORD_ELF,
        KEYWORD_ELSE,
        KEYWORD_LOOP,
        KEYWORD_SUBR,
        KEYWORD_CHAR,
        KEYWORD_SINT32,
        KEYWORD_UINT32,
        KEYWORD_SINT64,
        KEYWORD_UINT64,
        KEYWORD_FLOAT,
        KEYWORD_RETURN,
        
        // Literals
        LITERAL_CHAR,
        LITERAL_SINT32,
        LITERAL_UINT32,
        LITERAL_SINT64,
        LITERAL_UINT64,
        LITERAL_FLOAT,
        
        // Operators
        PLUS,
        MINUS,
        TIMES,
        INTEGER_DIVIDE,
        FLOAT_DIVIDE,
        MODULUS,
        EXPONENTIATION,
        LESS_THAN,
        GREATER_THAN,
        LESS_EQUAL,
        GREATER_EQUAL,
        COMPARE_EQUAL,
        NOT_EQUAL,
        LOGICAL_NOT,
        LOGICAL_OR,
        LOGICAL_XOR,
        LOGICAL_AND,
        ASSIGNMENT,
        
        // Delimiters
        ARROW,
        COLON,
        COMMA,
        LEFT_BRACE,
        RIGHT_BRACE,
        LEFT_PAREN,
        RIGHT_PAREN,
        SEMICOLON,
    };

    using type_t = Type;

    union Value {
        char          char_val;
        std::int32_t  sint32;
        std::uint32_t uint32;
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
