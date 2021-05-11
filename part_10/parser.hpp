#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "token.hpp"

class Parser {
   public:
    explicit Parser(Lexer lexer) : lexer_(lexer) { current_token_ = lexer_.getNextToken(); }

    std::shared_ptr<ASTNode> parse();

   private:
    // program : compund_statement DOT
    std::shared_ptr<ASTNode> program();

    // compound_statement : BEGIN statement_list END
    std::shared_ptr<CompoundNode> compound_statement();

    // statement_list: statement
    //               | statement SEMI statement_list
    std::vector<std::shared_ptr<ASTNode>> statement_list();

    // statement : compound_statement
    //           | assignment_statement
    //           | empty
    std::shared_ptr<ASTNode> statement();

    // assignment_statement : variable ASSIGN expr
    std::shared_ptr<AssignNode> assignment_statement();

    // variable : ID
    std::shared_ptr<VarNode> variable();

    // empty
    std::shared_ptr<NoOpNode> empty();

    // 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
    void eatToken(const TokenType &type);

    // factor: (PLUS | MINUS) factor | INTEGER | (LP expr RP) | variable
    std::shared_ptr<ASTNode> factor();

    // term: factor ((MUL | DIV) factor)*
    std::shared_ptr<ASTNode> term();

    // expr : term ((PLUS | MINUS) term)*
    std::shared_ptr<ASTNode> expr();

    Lexer lexer_;
    Token current_token_;
};

#endif
