/*
 *  Parser.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "parameters.hpp"
#include "token.hpp"
#include "Queue.hpp"

namespace sapphire {
    class Parser {
    public:
        struct Node {
            token_t data;
            Node* leaves[limits::max_leaves]{nullptr};
            Node* next{nullptr};
        };
        
        using node_t = Node;
        
        Parser();
       ~Parser();
        
        node_t* parse_program(Queue<token_t>& tokens);
        
    private:
        node_t* m_root;
        
        // Subroutine parsing
        node_t* parse_subr(Queue<token_t>& tokens);
        
        // Statement parsing
        node_t* parse_block(Queue<token_t>& tokens);
        node_t* parse_statement(Queue<token_t>& tokens);
        node_t* parse_if(Queue<token_t>& tokens);
        node_t* parse_loop(Queue<token_t>& tokens);
        node_t* parse_identifier_statement(Queue<token_t>& tokens);
        node_t* parse_declaration(Queue<token_t>& tokens);
        node_t* parse_return(Queue<token_t>& tokens);
        
        // Expression precedence ladder and expression parsing
        node_t* parse_expression(Queue<token_t>& tokens);
        node_t* parse_logical_or(Queue<token_t>& tokens);
        node_t* parse_logical_and(Queue<token_t>& tokens);
        node_t* parse_equality(Queue<token_t>& tokens);
        node_t* parse_comparison(Queue<token_t>& tokens);
        node_t* parse_additive(Queue<token_t>& tokens);
        node_t* parse_term(Queue<token_t>& tokens);
        node_t* parse_exponentiation(Queue<token_t>& tokens);
        node_t* parse_unary(Queue<token_t>& tokens);
        node_t* parse_factor(Queue<token_t>& tokens);
        
        // memory cleanup
        void free_ast(node_t* node);
    };
}

#endif // end PARSER_HPP
