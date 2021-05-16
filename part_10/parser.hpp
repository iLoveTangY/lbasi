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
    std::shared_ptr<ProgramNode> program();

    // block : declarations compund_statement
    std::shared_ptr<BlockNode> block();

    // compound_statement : BEGIN statement_list END
    std::shared_ptr<CompoundNode> compound_statement();

    // declarations : VAR (variable_delaration SEMI)+
    //              | empty
    std::vector<std::shared_ptr<VarDeclNode>> declaration();

    // variable_declaration : ID (COMMA ID)* COLON type_spec
    std::vector<std::shared_ptr<VarDeclNode>> variable_declaration();

    // type_spec : INTEGER
    //           | REAL
    std::shared_ptr<TypeNode> type_spec();

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
