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
    Parser::Parser() : m_root{nullptr} {}

    Parser::~Parser() {
        free_ast(m_root);
    }

    Parser::node_t* Parser::parse_program(Queue<token_t>& tokens) {
        node_t* head{nullptr};
        node_t* tail{nullptr};
        while (!tokens.is_empty() && tokens.peek().type != type_t::EOF_STREAM) {
            node_t* subr_node{parse_subr(tokens)};
            if (head == nullptr) {
                head = subr_node;
                tail = subr_node;
            } else {
                tail->next = subr_node;
                tail = subr_node;
            }
        }
        if (tokens.is_empty()) {
            throw std::runtime_error("sapphire::Parser::parse_program(): Runtime error. An EOF token was expected!");
        }
        tokens.de_queue(); // Remove the EOF token
        free_ast(m_root); // Necessary to prevent a memory leak in case parse_program is called twice on the same parse object
        m_root = head;
        return head;
    }

    Parser::node_t* Parser::parse_subr(Queue<token_t>& tokens) {
        if (tokens.peek().type != type_t::KEYWORD_SUBR) {
            throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. A 'subr' was expected!");
        }
        tokens.de_queue(); // Remove the 'subr'
        if (tokens.peek().type != type_t::COLON) {
            throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. A ':' was expected!");
        }
        tokens.de_queue(); // Remove the ':'
        if (tokens.peek().type != type_t::IDENTIFIER) {
            throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. An identifier was expected!");
        }
        node_t* subr_node{new node_t};
        subr_node->data = tokens.peek();
        tokens.de_queue(); // Remove the identifer name
        if (tokens.peek().type != type_t::LEFT_PAREN) {
            throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. A '(' was expected!");
        }
        tokens.de_queue(); // Remove the '('
        std::size_t index{2};
        while (tokens.peek().type != type_t::RIGHT_PAREN) {
            switch (tokens.peek().type) {
                case type_t::KEYWORD_CHAR:   [[fallthrough]];
                case type_t::KEYWORD_SINT32: [[fallthrough]];
                case type_t::KEYWORD_UINT32: [[fallthrough]];
                case type_t::KEYWORD_SINT64: [[fallthrough]];
                case type_t::KEYWORD_UINT64: [[fallthrough]];
                case type_t::KEYWORD_FLOAT:
                    break;
                default:
                    throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. Fundamental type expected!");
            }
            node_t* param_node{new node_t};
            param_node->data = tokens.peek();
            if (index >= limits::max_leaves) {
                throw std::out_of_range("sapphire::Parser::parse_subr(): Invalid token. Index is out of bounds!");
            }
            subr_node->leaves[index] = param_node;
            ++index;
            tokens.de_queue(); // Remove the fundamental type
            if (tokens.peek().type != type_t::IDENTIFIER) {
                throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. An identifier was expected!");
            }
            node_t* param_id{new node_t};
            param_id->data = tokens.peek();
            tokens.de_queue(); // remove the identifier name
            param_node->leaves[0] = param_id;
            if (tokens.peek().type == type_t::RIGHT_PAREN) {
                break;
            } else if (tokens.peek().type == type_t::COMMA) {
                tokens.de_queue(); // remove the ','
            } else {
                throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid argument!");
            }
        }
        tokens.de_queue(); // Remove the ')'
        if (tokens.peek().type == type_t::ARROW) {
            tokens.de_queue(); //Remove the '->'
            switch (tokens.peek().type) {
                case type_t::KEYWORD_CHAR:   [[fallthrough]];
                case type_t::KEYWORD_SINT32: [[fallthrough]];
                case type_t::KEYWORD_UINT32: [[fallthrough]];
                case type_t::KEYWORD_SINT64: [[fallthrough]];
                case type_t::KEYWORD_UINT64: [[fallthrough]];
                case type_t::KEYWORD_FLOAT:
                    break;
                default:
                    throw std::invalid_argument("sapphire::Parser::parse_subr(): Invalid token. Fundamental type expected!");
            }
            node_t* type_node{new node_t};
            type_node->data = tokens.peek();
            tokens.de_queue(); // Remove the fundamental type
            subr_node->leaves[0] = type_node;
        }
        subr_node->leaves[1] = parse_block(tokens);
        return subr_node;
    }

    Parser::node_t* Parser::parse_block(Queue<token_t>& tokens) {
        if (tokens.peek().type != type_t::LEFT_BRACE) {
            throw std::invalid_argument("sapphire::Parser::parse_block(): A '{' was expected!");
        }
        tokens.de_queue();
        node_t* head{nullptr};
        node_t* tail{nullptr};
        while (tokens.peek().type != type_t::RIGHT_BRACE) {
            if (tokens.peek().type == type_t::EOF_STREAM) {
                throw std::runtime_error("sapphire::Parser::parse_block(): A '}' was expected!");
            }
            node_t* stmt{parse_statement(tokens)};
            if (head == nullptr) {
                head = stmt;
                tail = stmt;
            } else {
                tail->next = stmt;
                tail = stmt;
            }
        }
        tokens.de_queue();
        return head;
    }
    
    Parser::node_t* Parser::parse_statement(Queue<token_t>& tokens) {
        switch (tokens.peek().type) {
            case type_t::KEYWORD_IF:
                return parse_if(tokens);
            case type_t::KEYWORD_LOOP:
                return parse_loop(tokens);
            case type_t::IDENTIFIER:
                return parse_identifier_statement(tokens);
            case type_t::KEYWORD_CHAR:   [[fallthrough]];
            case type_t::KEYWORD_SINT32: [[fallthrough]];
            case type_t::KEYWORD_UINT32: [[fallthrough]];
            case type_t::KEYWORD_SINT64: [[fallthrough]];
            case type_t::KEYWORD_UINT64: [[fallthrough]];
            case type_t::KEYWORD_FLOAT:
                return parse_declaration(tokens);
            case type_t::KEYWORD_RETURN:
                return parse_return(tokens);
            default:
                throw std::invalid_argument("sapphire::Parser::parse_statement(): Invalid token!");
        }
    }

    Parser::node_t* Parser::parse_if(Queue<token_t>& tokens) {
        if (tokens.peek().type != type_t::KEYWORD_IF &&
            tokens.peek().type != type_t::KEYWORD_ELF) {
            throw std::invalid_argument("sapphire::Parser::parse_if(): Invalid token. An 'if' or an 'elf' was expected!");
        }
        node_t* node{new node_t};
        node->data = tokens.peek();
        tokens.de_queue(); // Removes the 'if' of 'elf'
        if (tokens.peek().type != type_t::LEFT_PAREN) {
            throw std::invalid_argument("sapphire::Parser::parse_if(): Invalid token. A '(' was expected!");
        }
        tokens.de_queue(); // Removes the '('
        node->leaves[0] = parse_expression(tokens);
        if (tokens.peek().type != type_t::RIGHT_PAREN) {
            throw std::invalid_argument("sapphire::Parser::parse_if(): Invalid token. A ')' was expected!");
        }
        tokens.de_queue(); // Removes the ')'
        node->leaves[1] = parse_block(tokens);
        if (tokens.peek().type == type_t::KEYWORD_ELF) {
            node->leaves[2] = parse_if(tokens);
        } else if (tokens.peek().type == type_t::KEYWORD_ELSE)  {
            node_t* else_node{new node_t};
            else_node->data = tokens.peek();
            tokens.de_queue(); // Removes the 'else'
            else_node->leaves[0] = parse_block(tokens);
            node->leaves[2] = else_node;
        }
        return node;
    }

    Parser::node_t* Parser::parse_loop(Queue<token_t>& tokens) {
        if (tokens.peek().type != type_t::KEYWORD_LOOP) {
            throw std::invalid_argument("sapphire::Parser::parse_loop(): Invalid token. A 'loop' was expected!");
        }
        node_t* node{new node_t};
        node->data = tokens.peek();
        tokens.de_queue(); // Removes the 'loop'
        if (tokens.peek().type != type_t::LEFT_PAREN) {
            throw std::invalid_argument("sapphire::Parser::parse_loop(): Invalid token. A '(' was expected!");
        }
        tokens.de_queue(); // Removes the '('
        node->leaves[0] = parse_expression(tokens);
        if (tokens.peek().type != type_t::RIGHT_PAREN) {
            throw std::invalid_argument("sapphire::Parser::parse_loop(): Invalid token. A ')' was expected!");
        }
        tokens.de_queue(); // Removes the ')'
        node->leaves[1] = parse_block(tokens);
        return node;
    }

    Parser::node_t* Parser::parse_identifier_statement(Queue<token_t>& tokens) {
        if (tokens.peek().type != type_t::IDENTIFIER) {
            throw std::invalid_argument("sapphire::Parser::parse_identifier_statement(): Invalid token. An identifer was expected!");
        }
        node_t* id_node{new node_t};
        id_node->data = tokens.peek();
        tokens.de_queue(); // Remove the identifer name
        if (tokens.peek().type == type_t::ASSIGNMENT) {
            node_t* assignment_node{new node_t};
            assignment_node->data = tokens.peek();
            tokens.de_queue(); // Remove the '<-'
            assignment_node->leaves[0] = id_node;
            assignment_node->leaves[1] = parse_expression(tokens);
            if (tokens.peek().type != type_t::SEMICOLON) {
                throw std::invalid_argument("sapphire::Parser::parse_identifier_statement(): Invalid argument. A ';' was expected!");
            }
            tokens.de_queue(); // Remove the ';'
            return assignment_node;
        } else if (tokens.peek().type == type_t::LEFT_PAREN) {
            tokens.de_queue(); // Remove the '('
            std::size_t index{0};
            while (tokens.peek().type != type_t::RIGHT_PAREN) {
                if (index >= limits::max_leaves) {
                    throw std::out_of_range("sapphire::Parser::parse_identifier_statement(): Index is out of bounds!");
                }
                id_node->leaves[index] = parse_expression(tokens);
                ++index;
                if (tokens.peek().type == type_t::RIGHT_PAREN) {
                    break;
                } else if (tokens.peek().type == type_t::COMMA) {
                    tokens.de_queue(); // Remove the ','
                } else {
                    throw std::invalid_argument("sapphire::Parser::parse_identifier_statement(): A ',' was expected!");
                }
            }
            tokens.de_queue(); // Remove the ')'
            if (tokens.peek().type != type_t::SEMICOLON) {
                throw std::invalid_argument("sapphire::Parser::parse_identifier_statement(): A ';' was expected!");
            }
            tokens.de_queue(); // Remove the ';'
            return id_node;
        } else {
            throw std::invalid_argument("sapphire::Parser::parse_identifier_statement(): Invalid argument!");
        }
    }

    Parser::node_t* Parser::parse_declaration(Queue<token_t>& tokens) {
        switch (tokens.peek().type) {
            case type_t::KEYWORD_CHAR:   [[fallthrough]];
            case type_t::KEYWORD_SINT32: [[fallthrough]];
            case type_t::KEYWORD_UINT32: [[fallthrough]];
            case type_t::KEYWORD_SINT64: [[fallthrough]];
            case type_t::KEYWORD_UINT64: [[fallthrough]];
            case type_t::KEYWORD_FLOAT:
                break;
            default:
                throw std::invalid_argument("sapphire::Parser::parse_declaration(): Invalid token. Fundamental type expected!");
        }
        node_t* node{new node_t};
        node->data = tokens.peek();
        tokens.de_queue(); // Remove the fundamental type keyword
        if (tokens.peek().type != type_t::IDENTIFIER) {
            throw std::invalid_argument("sapphire::Parser::parse_declaration(): Invalid token. An identifier was expected!");
        }
        node_t* id_node{new node_t};
        id_node->data = tokens.peek();
        tokens.de_queue(); // Remove the identifier name
        node->leaves[0] = id_node;
        if (tokens.peek().type == type_t::ASSIGNMENT) { // Check fo optional initializer
            tokens.de_queue(); // Remove the '<-'
            node->leaves[1] = parse_expression(tokens);
        }
        if (tokens.peek().type != type_t::SEMICOLON) {
            throw std::invalid_argument("sapphire::Parser::parse_declaration(): Invalid token. A ';' was expected!");
        }
        tokens.de_queue(); // Remove the ';'
        return node;
    }

    Parser::node_t* Parser::parse_return(Queue<token_t>& tokens) {
        if (tokens.peek().type != type_t::KEYWORD_RETURN) {
            throw std::invalid_argument("sapphire::Parser::parse_return(): Invalid token. A 'return' was expected!");
        }
        node_t* node{new node_t};
        node->data = tokens.peek();
        tokens.de_queue(); // Remove the 'return'
        node->leaves[0] = parse_expression(tokens);
        if (tokens.peek().type != type_t::SEMICOLON) {
            throw std::invalid_argument("sapphire::Parser::parse_return(): Invalid token. A ';' was expected!");
        }
        tokens.de_queue(); // Remove the ';'
        return node;
    }

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
        if (token.type == type_t::LITERAL_CHAR   ||
            token.type == type_t::LITERAL_SINT32 ||
            token.type == type_t::LITERAL_UINT32 ||
            token.type == type_t::LITERAL_SINT64 ||
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
                throw std::invalid_argument("sapphire::Parser::parse_factor(): A ')' was expected!");
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
                    if (index >= limits::max_leaves) {
                        throw std::out_of_range("sapphire::Parser::parse_factor(): Index is out of bounds!");
                    }
                    node->leaves[index] = parse_expression(tokens);
                    ++index;
                    token = tokens.peek();
                    if (token.type == type_t::RIGHT_PAREN) {
                        break;
                    } else if (token.type == type_t::COMMA) {
                        tokens.de_queue();
                    } else {
                        throw std::invalid_argument("sapphire::Parser::parse_factor(): A ',' was expected!");
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

    void Parser::free_ast(node_t* node) {
        if (node == nullptr) {
            return;
        }
        for (std::size_t i{0}; i < limits::max_leaves; ++i) {
            free_ast(node->leaves[i]);
        }
        free_ast(node->next);
        delete node;
    }
}
