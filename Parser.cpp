/*
 *  Parser.cpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#include "Parser.hpp"
#include <stdexcept>

namespace sapphire {
    Parser::Parser() {}

    Parser::node_t* Parser::parse_expression(Queue<token_t>& tokens) {
        node_t* node{parse_logical_or(tokens)};
        return node;
    }

    Parser::node_t* Parser::parse_logical_or(Queue<token_t>& tokens) {
        node_t* node{parse_logical_and(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        while (next_token.type == type_t::LOGICAL_OR ||
               next_token.type == type_t::LOGICAL_XOR) {
            tokens.de_queue();
            node_t* temp_op{new node_t};
            if (op_node == nullptr) {
                op_node = temp_op;
                op_node->leaves[0] = node;
            } else {
                temp_op->leaves[0] = op_node;
                op_node = temp_op;
            }
            op_node->data = next_token;
            op_node->leaves[1] = parse_logical_and(tokens);
            next_token = tokens.peek();
        }
        if (op_node != nullptr) {
            return op_node;
        } else {
            return node;
        }
    }

    Parser::node_t* Parser::parse_logical_and(Queue<token_t>& tokens) {
        node_t* node{parse_equality(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        while (next_token.type == type_t::LOGICAL_AND) {
            tokens.de_queue();
            node_t* temp_op{new node_t};
            if (op_node == nullptr) {
                op_node = temp_op;
                op_node->leaves[0] = node;
            } else {
                temp_op->leaves[0] = op_node;
                op_node = temp_op;
            }
            op_node->data = next_token;
            op_node->leaves[1] = parse_equality(tokens);
            next_token = tokens.peek();
        }
        if (op_node != nullptr) {
            return op_node;
        } else {
            return node;
        }
    }

    Parser::node_t* Parser::parse_equality(Queue<token_t>& tokens) {
        node_t* node{parse_comparison(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        while (next_token.type == type_t::COMPARE_EQUAL ||
               next_token.type == type_t::NOT_EQUAL) {
            tokens.de_queue();
            node_t* temp_op{new node_t};
            if (op_node == nullptr) {
                op_node = temp_op;
                op_node->leaves[0] = node;
            } else {
                temp_op->leaves[0] = op_node;
                op_node = temp_op;
            }
            op_node->data = next_token;
            op_node->leaves[1] = parse_comparison(tokens);
            next_token = tokens.peek();
        }
        if (op_node != nullptr) {
            return op_node;
        } else {
            return node;
        }
    }

    Parser::node_t* Parser::parse_comparison(Queue<token_t>& tokens) {
        node_t* node{parse_additive(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        while (next_token.type == type_t::LESS_THAN ||
               next_token.type == type_t::LESS_EQUAL ||
               next_token.type == type_t::GREATER_THAN ||
               next_token.type == type_t::GREATER_EQUAL) {
            tokens.de_queue();
            node_t* temp_op{new node_t};
            if (op_node == nullptr) {
                op_node = temp_op;
                op_node->leaves[0] = node;
            } else {
                temp_op->leaves[0] = op_node;
                op_node = temp_op;
            }
            op_node->data = next_token;
            op_node->leaves[1] = parse_additive(tokens);
            next_token = tokens.peek();
        }
        if (op_node != nullptr) {
        return op_node;
        } else {
        return node;
        }
    }

    Parser::node_t* Parser::parse_additive(Queue<token_t>& tokens) {
        node_t* node{parse_term(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        while (next_token.type == type_t::PLUS ||
               next_token.type == type_t::MINUS) {
            tokens.de_queue();
            node_t* temp_op{new node_t};
            if (op_node == nullptr) {
                op_node = temp_op;
                op_node->leaves[0] = node;
            } else {
                temp_op->leaves[0] = op_node;
                op_node = temp_op;
            }
            op_node->data = next_token;
            op_node->leaves[1] = parse_term(tokens);
            next_token = tokens.peek();
        }
        if (op_node != nullptr) {
            return op_node;
        } else {
            return node;
        }
    }

    Parser::node_t* Parser::parse_term(Queue<token_t>& tokens) {
        node_t* node{parse_exponentiation(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        while (next_token.type == type_t::TIMES ||
               next_token.type == type_t::INTEGER_DIVIDE ||
               next_token.type == type_t::FLOAT_DIVIDE ||
               next_token.type == type_t::MODULUS) {
            tokens.de_queue();
            node_t* temp_op{new node_t};
            if (op_node == nullptr) {
                op_node = temp_op;
                op_node->leaves[0] = node;
            } else {
                temp_op->leaves[0] = op_node;
                op_node = temp_op;
            }
            op_node->data = next_token;
            op_node->leaves[1] = parse_exponentiation(tokens);
            next_token = tokens.peek();
        }
        if (op_node != nullptr) {
            return op_node;
        } else {
            return node;
        }
    }

    Parser::node_t* Parser::parse_exponentiation(Queue<token_t>& tokens) {
        node_t* node{parse_unary(tokens)};
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        if (next_token.type == type_t::EXPONENTIATION) {
            tokens.de_queue();
            op_node = new node_t;
            op_node->leaves[0] = node;
            op_node->data = next_token;
            op_node->leaves[1] = parse_exponentiation(tokens);
        }
        if (op_node != nullptr) {
            return op_node;
        } else {
            return node;
        }
    }

    Parser::node_t* Parser::parse_unary(Queue<token_t>& tokens) {
        node_t* op_node{nullptr};
        token_t next_token{tokens.peek()};
        if (next_token.type == type_t::LOGICAL_NOT ||
            next_token.type == type_t::MINUS ||
            next_token.type == type_t::PLUS) {
            tokens.de_queue();
            op_node = new node_t;
            op_node->data = next_token;
            op_node->leaves[0] = parse_unary(tokens);
        }
        if (op_node == nullptr) {
            op_node = parse_factor(tokens);
        }
        return op_node;
    }

    Parser::node_t* Parser::parse_factor(Queue<token_t>& tokens) {
        token_t token{tokens.peek()};
        node_t* node{nullptr};
        if (token.type == type_t::LITERAL_SINT64 ||
            token.type == type_t::LITERAL_UINT64 ||
            token.type == type_t::LITERAL_FLOAT) { // If true, token is an integer or a float
            node = new node_t;
            node->data = token;
            tokens.de_queue();
        } else if (token.type == type_t::LEFT_PAREN) { // If true, this is an expression within parentheses
            tokens.de_queue();
            node = parse_expression(tokens);
            token = tokens.peek();
            if (token.type == type_t::RIGHT_PAREN) {
                tokens.de_queue();
            } else {
                throw std::invalid_argument("sapphire::Parser::parse_factor(): Expected ')'!");
            }
        } else if (token.type == type_t::IDENTIFIER) { // If true, identifier could be a function or a variable
            node = new node_t;
            node->data = token;
            tokens.de_queue();
            token = tokens.peek();
            if (token.type == type_t::LEFT_PAREN) { // If true, identifier must be a function. Otherwise, it's a variable
                tokens.de_queue();
                token = tokens.peek();
                std::size_t index{0};
                while (token.type != type_t::RIGHT_PAREN) {
                    node->leaves[index] = parse_expression(tokens);
                    ++index;
                    token = tokens.peek();
                    if (token.type == type_t::RIGHT_PAREN) {
                        break;
                    }
                    if (token.type == type_t::COMMA) {
                        tokens.de_queue();
                    } else {
                        throw std::invalid_argument("sapphire::Parser::parse_factor(): A comma was expected!");
                    }
                    token = tokens.peek();
                }
                tokens.de_queue(); // Consume the right parenthesis
            }
        } else {
            throw std::invalid_argument("sapphire::Parser::parse_factor(): Unexpected token!");
        }
        return node;
    }
}
