#ifndef PARSER_HPP
#define PARSER_HPP

#include "error.hpp"
#include "lexer.hpp"
#include "token.hpp"

static void throwError(ErrorCode error_code, const Token &token) {
    throw ParserError(error_code, token, "");
}

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

    // declarations : (VAR (variable_delaration SEMI)+)? procedure_declaration*
    //              | empty
    std::vector<std::shared_ptr<ASTNode>> declaration();

    // procedure_declaration: PROCEDURE ID (LPAREN formal_parameter_list RPAREN)? SEMI block SEMI
    std::shared_ptr<ProcedureDecl> procedure_declaration();

    // formal_parameter_list : formal_parameters
    //                       | formal_parameters SEMI formal_parameter_list
    std::vector<std::shared_ptr<ParamNode>> formal_paramter_list();

    // formal_parameters : ID (COMMA ID)* COLON type_spec
    std::vector<std::shared_ptr<ParamNode>> formal_paramters();

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

    // ???????????? token ??? type ???????????? token_type???????????????????????? token
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
