#include "parser.hpp"

#define THROW_ERROR throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": Ivalid syntax")

std::shared_ptr<ASTNode> Parser::parse() {
    auto node = program();
    if (current_token_.type_ != END_OF_FILE) {
        THROW_ERROR;
    }
    return node;
}

// program : PROGRAM variable SEMI block DOT
std::shared_ptr<ProgramNode> Parser::program() {
    eatToken(PROGRAM);
    auto var_node = variable();
    auto program_name = var_node->value_;
    eatToken(SEMI);
    auto block_node = block();
    auto program_node = std::make_shared<ProgramNode>(program_name, block_node);
    eatToken(DOT);
    return program_node;
}

std::shared_ptr<BlockNode> Parser::block() {
    auto declarations_nodes = declaration();
    auto compund_statement_node = compound_statement();
    auto node = std::make_shared<BlockNode>(declarations_nodes, compund_statement_node);
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

// declarations : VAR (variable_delaration SEMI)+
//              | (PROCEDURE ID (LPPAREN formal_parameter_list RPAREN)? SEMI block SEMI)*
//              | empty
std::vector<std::shared_ptr<ASTNode>> Parser::declaration() {
    std::vector<std::shared_ptr<ASTNode>> declarations;
    if (current_token_.type_ == VAR) {
        eatToken(VAR);
        while (current_token_.type_ == ID) {
            auto var_decl = variable_declaration();
            declarations.insert(declarations.end(), var_decl.begin(), var_decl.end());
            eatToken(SEMI);
        }
    }
    while (current_token_.type_ == PROCEDURE) {
        eatToken(PROCEDURE);
        auto proc_name = current_token_.str_;
        eatToken(ID);
        std::vector<std::shared_ptr<ParamNode>> params;
        if (current_token_.type_ == LP) {
            eatToken(LP);
            params = formal_paramter_list();
            eatToken(RP);
        }
        eatToken(SEMI);
        auto block_node = block();
        declarations.emplace_back(std::make_shared<ProcedureDecl>(proc_name, params, block_node));
        eatToken(SEMI);
    }
    return declarations;
}

// formal_parameter_list : formal_parameters
//                       | formal_parameters SEMI formal_parameter_list
std::vector<std::shared_ptr<ParamNode>> Parser::formal_paramter_list() {
    if (current_token_.type_ != ID) {
        return {};
    }
    auto param_nodes = formal_paramters();

    while (current_token_.type_ == SEMI) {
        eatToken(SEMI);
        auto remain_params = formal_paramters();
        param_nodes.insert(param_nodes.end(), remain_params.begin(), remain_params.end());
    }
    return param_nodes;
}

// formal_parameters : ID (COMMA ID)* COLON type_spec
std::vector<std::shared_ptr<ParamNode>> Parser::formal_paramters() {
    std::vector<std::shared_ptr<ParamNode>> param_nodes;
    std::vector<Token> param_tokens = {current_token_};
    eatToken(ID);
    while (current_token_.type_ == COMMA) {
        eatToken(COMMA);
        param_tokens.push_back(current_token_);
        eatToken(ID);
    }
    eatToken(COLON);
    auto type_node = type_spec();

    for (const auto &param_token : param_tokens) {
        param_nodes.emplace_back(std::make_shared<ParamNode>(std::make_shared<VarNode>(param_token), type_node));
    }
    return param_nodes;
}

// variable_declaration : ID (COMMA ID)* COLON type_spec
std::vector<std::shared_ptr<VarDeclNode>> Parser::variable_declaration() {
    std::vector<std::shared_ptr<VarNode>> var_nodes;
    var_nodes.emplace_back(variable());

    while (current_token_.type_ == COMMA) {
        eatToken(COMMA);
        var_nodes.emplace_back(variable());
    }

    eatToken(COLON);

    auto type_node = type_spec();
    std::vector<std::shared_ptr<VarDeclNode>> var_declarations;
    for (auto &&var_node : var_nodes) {
        var_declarations.push_back(std::make_shared<VarDeclNode>(var_node, type_node));
    }
    return var_declarations;
}

// type_spec : INTEGER
//           | REAL
std::shared_ptr<TypeNode> Parser::type_spec() {
    auto token = current_token_;
    if (current_token_.type_ == INTEGER) {
        eatToken(INTEGER);
    } else {
        eatToken(REAL);
    }
    return std::make_shared<TypeNode>(token);
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

// factor: (PLUS | MINUS) factor | INTEGER_CONST | REAL_CONST | (LP expr RP) | variable
std::shared_ptr<ASTNode> Parser::factor() {
    auto token = current_token_;
    if (token.type_ == PLUS) {
        eatToken(PLUS);
        return std::make_shared<UnaryOpNode>(token, factor());
    } else if (token.type_ == MINUS) {
        eatToken(MINUS);
        return std::make_shared<UnaryOpNode>(token, factor());
    } else if (token.type_ == INTEGER_CONST) {
        eatToken(INTEGER_CONST);
        return std::make_shared<NumNode>(token);
    } else if (token.type_ == REAL_CONST) {
        eatToken(REAL_CONST);
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

// term : factor ((MUL | INTEGER_DIV | FLOAT_DIV) factor)*
std::shared_ptr<ASTNode> Parser::term() {
    auto node = factor();
    while (current_token_.type_ == MUL || current_token_.type_ == INTEGER_DIV || current_token_.type_ == FLOAT_DIV) {
        auto token = current_token_;
        if (token.type_ == MUL) {
            eatToken(MUL);
        } else if (token.type_ == INTEGER_DIV) {
            eatToken(INTEGER_DIV);
        } else if (token.type_ == FLOAT_DIV) {
            eatToken(FLOAT_DIV);
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