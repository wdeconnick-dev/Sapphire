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

#include "token.hpp"
#include "Queue.hpp"

namespace sapphire {
    class Parser {
    public:
        Parser();
        
    private:
        struct Node {
            token_t data;
            Node* leaves[12]{nullptr};
        };
        
        using node_t = Node;
        
        node_t* parse_expression(Queue<token_t>& tokens);
        node_t* parse_logical_or(Queue<token_t>& tokens);
        node_t* parse_logical_and(Queue<token_t>& tokens);
        node_t* parse_equality(Queue<token_t>& tokens);
        node_t* parse_comparison(Queue<token_t>& tokens);
        node_t* parse_additive(Queue<token_t>& tokens);
        node_t* parse_term(Queue<token_t>& tokens);
        node_t* parse_unary(Queue<token_t>& tokens);//
        node_t* parse_factor(Queue<token_t>& tokens);
    };
}

#endif // end PARSER_HPP
