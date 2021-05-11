#include "parser.hpp"

#define THROW_ERROR throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": Ivalid syntax")

std::shared_ptr<ASTNode> Parser::parse() {
    auto node = program();
    if (current_token_.type_ != END_OF_FILE) {
        THROW_ERROR;
    }
    return node;
}

// program : compund_statement DOT
std::shared_ptr<ASTNode> Parser::program() {
    auto node = compound_statement();
    eatToken(DOT);
    return node;
}

// compound_statement : BEGIN statement_list END
std::shared_ptr<CompoundNode> Parser::compound_statement() {
    eatToken(BEGIN);
    auto nodes = statement_list();
    eatToken(END);
    auto root = std::make_shared<CompoundNode>(nodes);
    return root;
}

// statement_list: statement
//               | statement SEMI statement_list
std::vector<std::shared_ptr<ASTNode>> Parser::statement_list() {
    auto node = statement();
    std::vector<std::shared_ptr<ASTNode>> results;
    results.push_back(node);
    while (current_token_.type_ == SEMI) {
        eatToken(SEMI);
        results.push_back(statement());
    }
    if (current_token_.type_ == ID) {
        THROW_ERROR;
    }
    return results;
}

// statement : compound_statement
//           | assignment_statement
//           | empty
std::shared_ptr<ASTNode> Parser::statement() {
    if (current_token_.type_ == BEGIN) {
        return compound_statement();
    } else if (current_token_.type_ == ID) {
        return assignment_statement();
    } else {
        return empty();
    }
}

// assignment_statement : variable ASSIGN expr
std::shared_ptr<AssignNode> Parser::assignment_statement() {
    auto left = variable();
    auto token = current_token_;
    eatToken(ASSIGN);
    auto right = expr();
    return std::make_shared<AssignNode>(left->value_, token, right);
}

// variable : ID
std::shared_ptr<VarNode> Parser::variable() {
    auto node = std::make_shared<VarNode>(current_token_);
    eatToken(ID);
    return node;
}

// empty
std::shared_ptr<NoOpNode> Parser::empty() {
    return std::make_shared<NoOpNode>();
}

// 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
void Parser::eatToken(const TokenType &type) {
    if (current_token_.type_ == type) {
        current_token_ = lexer_.getNextToken();
        return;
    }
    THROW_ERROR;
}

// factor: (PLUS | MINUS) factor | INTEGER | (LP expr RP) | variable
std::shared_ptr<ASTNode> Parser::factor() {
    auto token = current_token_;
    if (token.type_ == PLUS) {
        eatToken(PLUS);
        return std::make_shared<UnaryOpNode>(token, factor());
    } else if (token.type_ == MINUS) {
        eatToken(MINUS);
        return std::make_shared<UnaryOpNode>(token, factor());
    } else if (token.type_ == INTEGER) {
        eatToken(INTEGER);
        return std::make_shared<NumNode>(token);
    } else if (token.type_ == LP) {
        eatToken(LP);
        auto node = expr();
        eatToken(RP);
        return node;
    } else {
        return variable();
    }
    THROW_ERROR;
    return nullptr;
}

// term: factor ((MUL | DIV) factor)*
std::shared_ptr<ASTNode> Parser::term() {
    auto node = factor();
    while (current_token_.type_ == MUL || current_token_.type_ == DIV) {
        auto token = current_token_;
        if (token.type_ == MUL) {
            eatToken(MUL);
        } else if (token.type_ == DIV) {
            eatToken(DIV);
        }
        node = std::make_shared<BinaryOpNode>(node, token, factor());
    }

    return node;
}

// expr : term ((PLUS | MINUS) term)*
std::shared_ptr<ASTNode> Parser::expr() {
    auto node = term();

    while (current_token_.type_ == PLUS || current_token_.type_ == MINUS) {
        auto token = current_token_;
        if (token.type_ == PLUS) {
            eatToken(PLUS);
        } else if (token.type_ == MINUS) {
            eatToken(MINUS);
        }
        node = std::make_shared<BinaryOpNode>(node, token, term());
    }
    return node;
}