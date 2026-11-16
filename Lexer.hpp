/*
 *  Lexer.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"
#include "utility.hpp"
#include "Queue.hpp"
#include "String.hpp"
#include <cstddef>

namespace sapphire {
    class Lexer {
    public:
        Lexer();
        
        void next_token(const String& source, token_t& token, std::size_t& index);
        void tokenize(Queue<token_t>& tokens);
        
    private:
        std::size_t m_line;
        
        void extract_identifier(const String& source, token_t& token, std::size_t& index);
        void extract_number(const String& source, token_t& token, std::size_t& index);
        void extract_operator(const String& source, token_t& token, std::size_t& index);
        void resolve_identifier(const String& identifier, token_t& token);
        void resolve_operator(const String& operators, token_t& token);
    };
}

#endif // end LEXER_HPP
