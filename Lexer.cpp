/*
 *  Lexer.cpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#include "Lexer.hpp"
#include "utility.hpp"
#include <stdexcept>

namespace sapphire {
    Lexer::Lexer() : m_line{1} {}

    void Lexer::next_token(const String& source, token_t& token, std::size_t& index) {
        if (index >= source.length()) {
            throw std::out_of_range("sapphire::Lexer::next_token(): Index is out of bounds!");
        }
        if (is_alpha(source[index]) || is_underscore(source[index])) {
            extract_identifier(source, token, index);
        } else if (is_digit(source[index]) || is_decimal_point(source[index])) {
            extract_number(source, token, index);
        } else if (is_operator(source[index])) {
            extract_operator(source, token, index);
        } else {
            throw std::invalid_argument("sapphire::Lexer::next_token(): Invalid input!");
        }
    }

    void Lexer::tokenize(Queue<token_t>& tokens) {
        String source{};
        bool EOF_flag{false};
        get_input(source, limits::terminal_delimiter, EOF_flag);
        std::size_t index{0};
        while (index < source.length() && source[index] != limits::terminal_delimiter) {
            if (is_newline(source[index])) {
                ++m_line;
                ++index;
                continue;
            }
            if (is_whitespace(source[index])) {
                ++index;
                continue;
            }
            token_t token{};
            next_token(source, token, index);
            tokens.en_queue(token);
        }
        token_t eof_token{};
        eof_token.type = type_t::EOF_STREAM;
        eof_token.line_count = m_line;
        tokens.en_queue(eof_token);
    }

    void Lexer::extract_identifier(const String& source, token_t& token, std::size_t& index) {
        if (index >= source.length()) {
            throw std::out_of_range("sapphire::Lexer::extract_identifier(): Index is out of bounds!");
        }
        if (!(is_alpha(source[index]) || is_underscore(source[index]))) {
            throw std::invalid_argument("sapphire::Lexer::extract_identifier(): Invalid argument. An alpha or an '_' was expected!");
        }
        String identifier{};
        get_identifier(identifier, source, index);
        token.lexeme = identifier;
        resolve_identifier(identifier, token);
        token.line_count = m_line;
    }

    void Lexer::extract_number(const String& source, token_t& token, std::size_t& index) {
        if (index >= source.length()) {
            throw std::out_of_range("sapphire::Lexer::extract_number(): Index is out of bounds!");
        }
        if (!(is_digit(source[index]) || is_decimal_point(source[index]))) {
            throw std::invalid_argument("sapphire::Lexer::extract_number(): Invalid argument. A digit or a '.' was expected!");
        }
        String digits{};
        number_type_t number_type{};
        get_number(digits, source, index, number_type);
        token.lexeme = digits;
        if (number_type == number_type_t::UNSIGNED_INTEGER_TYPE) {
            token.type = type_t::LITERAL_UINT64;
            token.value.uint64 = chars_to_int(digits);
        } else if (number_type == number_type_t::FLOAT_TYPE) {
            token.type = type_t::LITERAL_FLOAT;
            token.value.float_val = chars_to_float(digits);
        }
        token.line_count = m_line;
    }

    void Lexer::extract_operator(const String& source, token_t& token, std::size_t& index) {
        if (index >= source.length()) {
            throw std::out_of_range("sapphire::Lexer::extract_operator(): Index is out of bounds!");
        }
        if (!is_operator(source[index])) {
            throw std::invalid_argument("sapphire::Lexer::extract_operator(): Invalid argument. An operator was expected!");
        }
        String operators{};
        get_operator(operators, source, index);
        token.lexeme = operators;
        resolve_operator(operators, token);
        token.line_count = m_line;
    }

    void Lexer::resolve_identifier(const String& identifier, token_t& token) {
        if (identifier.is_empty()) {
            throw std::invalid_argument("sapphire::Lexer::resolve_identifier(): Invalid argument. An identifier cannot be empty!");
        }
        if (identifier == "if") {
            token.type = type_t::KEYWORD_IF;
        } else if (identifier == "elf") {
            token.type = type_t::KEYWORD_ELF;
        } else if (identifier == "else") {
            token.type = type_t::KEYWORD_ELSE;
        } else if (identifier == "loop") {
            token.type = type_t::KEYWORD_LOOP;
        } else if (identifier == "subr") {
            token.type = type_t::KEYWORD_SUBR;
        } else if (identifier == "var_sint64") {
            token.type = type_t::KEYWORD_VAR_SINT64;
        } else if (identifier == "var_uint64") {
            token.type = type_t::KEYWORD_VAR_UINT64;
        } else if (identifier == "var_float") {
            token.type = type_t::KEYWORD_VAR_FLOAT;
        } else if (identifier == "return") {
            token.type = type_t::KEYWORD_RETURN;
        } else {
            token.type = type_t::IDENTIFIER;
        }
    }

    void Lexer::resolve_operator(const String& operators, token_t& token) {
        if (operators.is_empty()) {
            throw std::invalid_argument("sapphire::Lexer::resolve_operator(): Invalid argument. An operator was expected!");
        }
        if (operators == "+") {
            token.type = type_t::PLUS;
        } else if (operators == "-") {
            token.type = type_t::MINUS;
        } else if (operators == "*") {
            token.type = type_t::TIMES;
        } else if (operators == "/") {
            token.type = type_t::INTEGER_DIVIDE;
        } else if (operators == "//") {
            token.type = type_t::FLOAT_DIVIDE;
        } else if (operators == "%") {
            token.type = type_t::MODULUS;
        } else if (operators == "^") {
            token.type = type_t::EXPONENTIATION;
        } else if (operators == "<") {
            token.type = type_t::LESS_THAN;
        } else if (operators == ">") {
            token.type = type_t::GREATER_THAN;
        } else if (operators == "<=") {
            token.type = type_t::LESS_EQUAL;
        } else if (operators == ">=") {
            token.type = type_t::GREATER_EQUAL;
        } else if (operators == "=") {
            token.type = type_t::COMPARE_EQUAL;
        } else if (operators == "!=") {
            token.type = type_t::NOT_EQUAL;
        } else if (operators == "!") {
            token.type = type_t::LOGICAL_NOT;
        } else if (operators == "||") {
            token.type = type_t::LOGICAL_XOR;
        } else if (operators == "&|") {
            token.type = type_t::LOGICAL_OR;
        } else if (operators == "&&") {
            token.type = type_t::LOGICAL_AND;
        } else if (operators == "<-") {
            token.type = type_t::ASSIGNMENT;
        } else {
            throw std::invalid_argument("sapphire::Lexer::resolve_operator(): Invalid argument. Operator sequence not recognized!");
        }
    }
}
